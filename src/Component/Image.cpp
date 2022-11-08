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
                : Base({Property::Type::Image, name}) {
        }

        Image::Image(std::string name, Transform transform,
                     cairo_surface_t *_imageSurface,
                     double width, double height)
                : Base({Property::Type::Image, name,
                        (width || height) ? width : cairo_image_surface_get_width(_imageSurface),
                        (width || height) ? height : cairo_image_surface_get_height(_imageSurface)},
                       transform),
                  imageSurface(_imageSurface) {
            if (width || height)
                cairo_scale(cr,
                            width / (double) cairo_image_surface_get_width(imageSurface),
                            height / (double) cairo_image_surface_get_height(imageSurface));
            cairo_set_source_surface(cr, imageSurface, 0, 0);
            cairo_paint(cr);
        }

        Image Image::fromRaw(std::string name, Transform transform,
                             unsigned char *imageData,
                             int imageCols, int imageRows,
                             int imageStride,
                             ColorFormat colorFormat,
                             double width, double height) {
            // 计算预乘
            if (colorFormat == ColorFormat::ARGB32NoPremultiplied) {
                // 尝试 cairo_set_operator CAIRO_OPERATOR_OVER CAIRO_OPERATOR_SOURCE ?
                for (int y = 0; y < imageRows; y++)
                    for (int x = 0; x < imageCols; x++) {
                        auto p = imageData + y * imageStride + x * 4;
                        p[0] = (unsigned short) p[0] * p[3] / 0xff;
                        p[1] = (unsigned short) p[1] * p[3] / 0xff;
                        p[2] = (unsigned short) p[2] * p[3] / 0xff;
                    }
                colorFormat = ColorFormat::ARGB32;
            }

            cairo_surface_t *imageSurface = cairo_image_surface_create_for_data(imageData,
                                                                                toCairoFormat(colorFormat),
                                                                                imageCols,
                                                                                imageRows,
                                                                                imageStride);

            Image ret = Image(name, transform,
                              imageSurface,
                              width, height);

            cairo_surface_destroy(imageSurface); // 回收

            return ret;
        }

#ifdef OpenCV_FOUND

        Image Image::fromMat(std::string name, Transform transform,
                             const cv::Mat &imageMat,
                             double width, double height) {
            if (imageMat.channels() == 1)
                return Image::fromRaw(name, transform,
                                      imageMat.data, imageMat.cols, imageMat.rows, imageMat.step, ColorFormat::A8,
                                      width, height);
            else if (imageMat.channels() == 3)
                return Image::fromRaw(name, transform,
                                      imageMat.data, imageMat.cols, imageMat.rows, imageMat.step, ColorFormat::RGB24,
                                      width, height);
            else if (imageMat.channels() == 4)
                return Image::fromRaw(name, transform,
                                      imageMat.data, imageMat.cols, imageMat.rows, imageMat.step, ColorFormat::ARGB32NoPremultiplied,
                                      width, height);
            else
                return Image(name);
        }

#endif

#ifdef OpenCV_FOUND

        Image Image::fromFileByCV(std::string name, Transform transform,
                                  const std::string &imageFilePath,
                                  double width, double height) {
            return Image::fromMat(name, transform,
                                  cv::imread(imageFilePath, cv::IMREAD_UNCHANGED),
                                  width, height);
        }

#endif

        Image Image::fromPng(std::string name, Transform transform,
                             const std::string &imageFilePath,
                             double width, double height) {
            if (std::filesystem::exists(imageFilePath))
                return Image(name, transform, cairo_image_surface_create_from_png(imageFilePath.c_str()), width, height);
            return Image(name);
        }

#ifdef JPEG_FOUND

        Image Image::fromJpg(std::string name, Transform transform,
                             const std::string &filename,
                             double width, double height) {
            FILE *infile;
            if ((infile = fopen(filename.c_str(), "rb")) == nullptr) {
                fprintf(stderr, "文件不存在： %s \n", filename.c_str());
                return Image(name);
            }

            //准备
            jpeg_decompress_struct cinfo;
            jpeg_error_mgr error_mgr; //出错处理
            cinfo.err = jpeg_std_error(&error_mgr);
            error_mgr.error_exit = [](j_common_ptr cinfo) {
                (*cinfo->err->output_message)(cinfo);
            };
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, infile);
            (void) jpeg_read_header(&cinfo, true);
            (void) jpeg_start_decompress(&cinfo);

            int row_stride = cinfo.output_width * cinfo.output_components;
            JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

            // 申请内存
            uint8_t *image_buffer = (uint8_t *) malloc(cinfo.output_width * cinfo.output_height * 4);

            //开始读取
            uint8_t *image_buffer_p = image_buffer;
            while (cinfo.output_scanline < cinfo.output_height) {
                (void) jpeg_read_scanlines(&cinfo, buffer, 1);
                memcpy(image_buffer_p, buffer[0], row_stride); // todo
                for (int i = 0; i < cinfo.output_width; i++) {
                    image_buffer_p[i * 4 + 1] = buffer[0][i * cinfo.output_components + 0];
                    image_buffer_p[i * 4 + 2] = buffer[0][i * cinfo.output_components + 1];
                    image_buffer_p[i * 4 + 3] = buffer[0][i * cinfo.output_components + 2];
                }
                image_buffer_p += cinfo.output_width * 4;
                // put_scanline_someplace(buffer[0], row_stride);
            }

            auto ret = Image::fromRaw(name, transform,
                                      image_buffer, cinfo.output_width, cinfo.output_height, cinfo.output_width * 4, ColorFormat::ARGB32,
                                      width, height);

            free(image_buffer);

            //清理
            (void) jpeg_finish_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);

            return ret;
        }

#endif

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
            return Image(name);
        }

        Image::~Image() {
            finish();
            if (imageSurface)
                cairo_surface_destroy(imageSurface);
        }

    } // namespace Component
} // namespace ArtRobot
