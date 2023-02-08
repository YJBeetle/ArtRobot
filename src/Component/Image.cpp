/*
 * Copyright 2022 YJBeetle
 *
 * Authors:
 *  YJBeetle <YJBeetle@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 3.  See
 * the LICENSE file in the top-level directory.
 *
 */

#include "./Image.hpp"

#include <fstream>
#include <iostream>

#ifdef JPEG_FOUND

#include <jpeglib.h>

#endif

namespace ArtRobot {
    namespace Component {

        Image::Image(std::string name)
                : Base({name}, {}) {
        }

        Image::Image(std::string name, Transform transform,
                     cairo_surface_t *_imageSurface,
                     double width, double height)
                : Base({name,
                        !isnan(width) ? width : !isnan(height) ? (height / cairo_image_surface_get_height(_imageSurface) * cairo_image_surface_get_width(_imageSurface)) : cairo_image_surface_get_width(_imageSurface),
                        !isnan(height) ? height : !isnan(width) ? (width / cairo_image_surface_get_width(_imageSurface) * cairo_image_surface_get_height(_imageSurface)) : cairo_image_surface_get_height(_imageSurface)},
                       transform),
                  imageSurface(_imageSurface),
                  imageCols(cairo_image_surface_get_width(imageSurface)),
                  imageRows(cairo_image_surface_get_height(imageSurface)) {
            if (!isnan(width) || !isnan(height))
                cairo_scale(cr,
                            property.width / imageCols,
                            property.height / imageRows);
            cairo_set_source_surface(cr, imageSurface, 0, 0);
            cairo_paint(cr);
        }

        Image Image::fromRaw(std::string name, Transform transform,
                             unsigned char *imageData,
                             int imageCols, int imageRows,
                             int imageStride, bool isPremultiplied,
                             double width, double height) {
            // 计算预乘
            if (!isPremultiplied) {
                // 尝试 cairo_set_operator CAIRO_OPERATOR_OVER CAIRO_OPERATOR_SOURCE ?
                for (int y = 0; y < imageRows; y++)
                    for (int x = 0; x < imageCols; x++) {
                        auto p = imageData + y * imageStride + x * 4;
                        p[0] = (unsigned short) p[0] * p[3] / 0xff;
                        p[1] = (unsigned short) p[1] * p[3] / 0xff;
                        p[2] = (unsigned short) p[2] * p[3] / 0xff;
                    }
            }

            cairo_surface_t *imageSurface = cairo_image_surface_create_for_data(imageData,
                                                                                CAIRO_FORMAT_ARGB32,
                                                                                imageCols,
                                                                                imageRows,
                                                                                imageStride);

            return Image(name, transform,
                         imageSurface,
                         width, height);
        }

#ifdef OpenCV_FOUND

        Image Image::fromMat(std::string name, Transform transform,
                             const cv::Mat &imageMat,
                             double width, double height) {
            if (imageMat.channels() == 1)
                return Image(name); // todo
            else if (imageMat.channels() == 3)
                return Image(name); // todo
            else if (imageMat.channels() == 4)
                return Image::fromRaw(name, transform,
                                      imageMat.data,
                                      imageMat.cols, imageMat.rows,
                                      imageMat.step, false,
                                      width, height);
            else
                return Image(name);
        }

#endif

#ifdef OpenCV_FOUND
#ifndef OpenCV_WITHOUT_IMAPI

        Image Image::fromFileByCV(std::string name, Transform transform,
                                  const std::string &imageFilePath,
                                  double width, double height) {
            return Image::fromMat(name, transform,
                                  cv::imread(imageFilePath, cv::IMREAD_UNCHANGED),
                                  width, height);
        }

#endif
#endif

        Image Image::fromPng(std::string name, Transform transform,
                             const std::vector<uint8_t> &data,
                             double width, double height) {
            struct PngStreamClosure {
                const uint8_t *data;
                const size_t max_size;
                size_t pos;
            } pngStreamClosure{data.data(), data.size(), 0};
            return Image(name, transform,
                         cairo_image_surface_create_from_png_stream(
                                 [](void *_closure,
                                    unsigned char *data,
                                    unsigned int length) -> cairo_status_t {
                                     PngStreamClosure *closure = (PngStreamClosure *) _closure;
                                     if ((closure->pos + length) > (closure->max_size))
                                         return CAIRO_STATUS_READ_ERROR;
                                     memcpy(data, (closure->data + closure->pos), length);
                                     closure->pos += length;
                                     return CAIRO_STATUS_SUCCESS;
                                 }, (void *) &pngStreamClosure),
                         width, height);
        }

        Image Image::fromPng(std::string name, Transform transform,
                             const std::string &imageFilePath,
                             double width, double height) {
            if (std::filesystem::exists(imageFilePath))
                return Image(name, transform, cairo_image_surface_create_from_png(imageFilePath.c_str()), width, height);
            return Image(name);
        }

#ifdef JPEG_FOUND

        class JepgReader {
        private:
            jpeg_decompress_struct cInfo;
            jpeg_error_mgr errorMgr; //出错处理
            cairo_surface_t *imageSurface;
        public:
            JepgReader() {
                cInfo.err = jpeg_std_error(&errorMgr);
                errorMgr.error_exit = [](j_common_ptr cInfo) {
                    (*cInfo->err->output_message)(cInfo);
                };
                jpeg_create_decompress(&cInfo);
            }

            ~JepgReader() {
                (void) jpeg_finish_decompress(&cInfo);
                jpeg_destroy_decompress(&cInfo);
            }

            void loadFromMem(const unsigned char *inBuffer, unsigned long inSize) {
                jpeg_mem_src(&cInfo, inBuffer, inSize);
            }

            void loadFromStdio(FILE *inFile) {
                jpeg_stdio_src(&cInfo, inFile);
            }

            bool read() {
                (void) jpeg_read_header(&cInfo, true);
                (void) jpeg_start_decompress(&cInfo);

                int row_stride = cInfo.output_width * cInfo.output_components;
                JSAMPARRAY buffer = (*cInfo.mem->alloc_sarray)((j_common_ptr) &cInfo, JPOOL_IMAGE, row_stride, 1);

                imageSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, cInfo.output_width, cInfo.output_height);
                auto imageSurfaceData = cairo_image_surface_get_data(imageSurface);
                auto imageSurfaceStride = cairo_image_surface_get_stride(imageSurface);

                if (cInfo.output_components == 1) {
                    while (cInfo.output_scanline < cInfo.output_height) {
                        (void) jpeg_read_scanlines(&cInfo, buffer, 1);
                        for (int col = 0; col < cInfo.output_width; col++) {
                            size_t ofsI = col * 4;
                            imageSurfaceData[ofsI + 0] = imageSurfaceData[ofsI + 1] = imageSurfaceData[ofsI + 2] = buffer[0][col];
                        }
                        imageSurfaceData += imageSurfaceStride;
                    }
                } else if (cInfo.output_components == 3 || cInfo.output_components == 4) {
                    while (cInfo.output_scanline < cInfo.output_height) {
                        (void) jpeg_read_scanlines(&cInfo, buffer, 1);
                        for (int col = 0; col < cInfo.output_width; col++) {
                            size_t ofsI = col * 4;
                            size_t ofsJ = col * cInfo.output_components;
                            imageSurfaceData[ofsI + 0] = buffer[0][ofsJ + 2];
                            imageSurfaceData[ofsI + 1] = buffer[0][ofsJ + 1];
                            imageSurfaceData[ofsI + 2] = buffer[0][ofsJ + 0];
                        }
                        imageSurfaceData += imageSurfaceStride;
                    }
                }

                return true;
            }

            cairo_surface_t *surface() {
                return imageSurface;
            }
        };

        Image Image::fromJpg(std::string name, Transform transform,
                             const std::vector<uint8_t> &data,
                             double width, double height) {
            JepgReader r;
            r.loadFromMem(data.data(), data.size());
            if (r.read())
                return Image(name, transform,
                             r.surface(),
                             width, height);
            else
                return Image(name);
        }

        Image Image::fromJpg(std::string name, Transform transform,
                             const std::string &filename,
                             double width, double height) {
            FILE *inFile;
            if ((inFile = fopen(filename.c_str(), "rb")) == nullptr)
                return Image(name);
            JepgReader r;
            r.loadFromStdio(inFile);
            auto rr = r.read();
            fclose(inFile);
            if (rr)
                return Image(name, transform,
                             r.surface(),
                             width, height);
            else
                return Image(name);
        }

#endif


        Image Image::fromFile(std::string name, Transform transform,
                              const std::vector<uint8_t> &data,
                              double width, double height) {
            if (memcmp(data.data(), (uint8_t[]) {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}, 8) == 0)
                return Image::fromPng(name, transform, data, width, height);
#ifdef JPEG_FOUND
            else if (memcmp(data.data(), (uint8_t[]) {0xFF, 0xD8, 0xFF}, 3) == 0)
                return Image::fromJpg(name, transform, data, width, height);
#endif
            else
                return Image(name);
        }

        Image Image::fromFile(std::string name, Transform transform,
                              const std::string &imageFilePath,
                              double width, double height) {
            const char *ext = imageFilePath.c_str() + imageFilePath.length() - 4;
            if (!strcasecmp(ext, ".png"))
                return Image::fromPng(name, transform, imageFilePath, width, height);
#ifdef JPEG_FOUND
            else if (!strcasecmp(ext, ".jpg"))
                return Image::fromJpg(name, transform, imageFilePath, width, height);
#endif
            else
                return Image(name);
        }

        Image::~Image() {
            if (imageSurface)
                cairo_surface_destroy(imageSurface);
        }

    } // namespace Component
} // namespace ArtRobot
