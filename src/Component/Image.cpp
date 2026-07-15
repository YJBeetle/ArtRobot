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

#include <csetjmp>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#ifdef JPEG_FOUND

#include <jpeglib.h>

#endif

namespace ArtRobot {
    namespace Component {

        Image::Image(std::string name)
                : Base({name}, {}) {
        }

        Image::Image(std::string name, Transform transform,
                     unsigned char *imageData,
                     int imageColums, int imageRows,
                     int imageStride, bool isPremultiplied,
                     double width, double height)
                : Image(name, transform, surfaceFromRaw(imageData, imageColums, imageRows, imageStride, isPremultiplied), width, height) {
        }

        Image::Image(std::string name, Transform transform,
                     const std::vector<uint8_t> &data,
                     double width, double height)
                : Image(name, transform, validateSurface(surfaceFromFile(data)), width, height) {
        }

        Image::Image(std::string name, Transform transform,
                     const std::string &filename,
                     double width, double height)
                : Image(name, transform, validateSurface(surfaceFromFile(filename)), width, height) {
        }

#ifdef OpenCV_FOUND

        Image::Image(std::string name, Transform transform,
                     const cv::Mat &imageMat,
                     double width, double height)
                : Image(name, transform, surfaceFromRaw(imageMat.data, imageMat.cols, imageMat.rows, imageMat.step, false), width, height) {
        }

#endif

        Image::~Image() {
            if (imageSurface)
                cairo_surface_destroy(imageSurface);
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

        cairo_surface_t *Image::validateSurface(cairo_surface_t *surface) {
            if (!surface)
                throw std::invalid_argument("Unsupported image source");
            const auto status = cairo_surface_status(surface);
            if (status != CAIRO_STATUS_SUCCESS) {
                const std::string message = cairo_status_to_string(status);
                cairo_surface_destroy(surface);
                throw std::runtime_error("Failed to decode image: " + message);
            }
            return surface;
        }

        cairo_surface_t *Image::surfaceFromRaw(unsigned char *imageData,
                                               int imageCols, int imageRows,
                                               int imageStride, bool isPremultiplied) {
            if (!imageData || imageCols <= 0 || imageRows <= 0 ||
                imageStride < cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, imageCols))
                throw std::invalid_argument("Invalid raw image buffer");

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

            return cairo_image_surface_create_for_data(imageData,
                                                       CAIRO_FORMAT_ARGB32,
                                                       imageCols,
                                                       imageRows,
                                                       imageStride);
        }

        cairo_surface_t *Image::surfaceFromPng(const std::vector<uint8_t> &data) {
            struct PngStreamClosure {
                const uint8_t *data;
                const size_t max_size;
                size_t pos;
            } pngStreamClosure{data.data(), data.size(), 0};
            return cairo_image_surface_create_from_png_stream(
                    [](void *_closure,
                       unsigned char *data,
                       unsigned int length) -> cairo_status_t {
                        PngStreamClosure *closure = (PngStreamClosure *) _closure;
                        if ((closure->pos + length) > (closure->max_size))
                            return CAIRO_STATUS_READ_ERROR;
                        memcpy(data, (closure->data + closure->pos), length);
                        closure->pos += length;
                        return CAIRO_STATUS_SUCCESS;
                    }, (void *) &pngStreamClosure);
        }

        cairo_surface_t *Image::surfaceFromPng(const std::string &filename) {
            if (std::filesystem::exists(filename))
                return cairo_image_surface_create_from_png(filename.c_str());
            return nullptr;
        }

#ifdef JPEG_FOUND

        class JpegReader {
        private:
            struct ErrorManager {
                jpeg_error_mgr base;
                std::jmp_buf jumpBuffer;
                char message[JMSG_LENGTH_MAX]{};
            } errorManager;

            jpeg_decompress_struct cInfo;
            cairo_surface_t *imageSurface = nullptr;

        public:
            JpegReader() {
                cInfo.err = jpeg_std_error(&errorManager.base);
                errorManager.base.error_exit = [](j_common_ptr cInfo) {
                    auto *manager = reinterpret_cast<ErrorManager *>(cInfo->err);
                    (*cInfo->err->format_message)(cInfo, manager->message);
                    std::longjmp(manager->jumpBuffer, 1);
                };
                jpeg_create_decompress(&cInfo);
            }

            ~JpegReader() {
                if (imageSurface)
                    cairo_surface_destroy(imageSurface);
                jpeg_destroy_decompress(&cInfo);
            }

            void loadFromMem(const unsigned char *inBuffer, unsigned long inSize) {
                jpeg_mem_src(&cInfo, inBuffer, inSize);
            }

            void loadFromStdio(FILE *inFile) {
                jpeg_stdio_src(&cInfo, inFile);
            }

            cairo_surface_t *read() {
                if (setjmp(errorManager.jumpBuffer))
                    throw std::runtime_error("Failed to decode JPEG: " + std::string(errorManager.message));

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

                (void) jpeg_finish_decompress(&cInfo);
                auto *result = imageSurface;
                imageSurface = nullptr;
                return result;
            }
        };

        cairo_surface_t *Image::surfaceFromJpg(const std::vector<uint8_t> &data) {
            JpegReader r;
            r.loadFromMem(data.data(), data.size());
            return r.read();
        }

        cairo_surface_t *Image::surfaceFromJpg(const std::string &filename) {
            FILE *inFile;
            if ((inFile = fopen(filename.c_str(), "rb")) == nullptr)
                return nullptr;
            JpegReader r;
            r.loadFromStdio(inFile);
            auto rr = r.read();
            fclose(inFile);
            return rr;
        }

#endif

        cairo_surface_t *Image::surfaceFromFile(const std::vector<uint8_t> &data) {
            static const uint8_t pngSignature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
            static const uint8_t jpegSignature[] = {0xFF, 0xD8, 0xFF};
            if (data.size() >= sizeof(pngSignature) && memcmp(data.data(), pngSignature, sizeof(pngSignature)) == 0)
                return surfaceFromPng(data);
#ifdef JPEG_FOUND
            else if (data.size() >= sizeof(jpegSignature) && memcmp(data.data(), jpegSignature, sizeof(jpegSignature)) == 0)
                return surfaceFromJpg(data);
#endif
            return nullptr;
        }

        cairo_surface_t *Image::surfaceFromFile(const std::string &filename) {
            const auto extension = std::filesystem::path(filename).extension().string();
            if (!strcasecmp(extension.c_str(), ".png"))
                return surfaceFromPng(filename);
#ifdef JPEG_FOUND
            else if (!strcasecmp(extension.c_str(), ".jpg") || !strcasecmp(extension.c_str(), ".jpeg"))
                return surfaceFromJpg(filename);
#endif
            return nullptr;
        }

    } // namespace Component
} // namespace ArtRobot
