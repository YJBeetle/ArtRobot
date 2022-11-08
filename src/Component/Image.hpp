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

#pragma once

#include <string>

#include <ArtRobot/Features.hpp>

#ifdef OpenCV_FOUND

#include <opencv2/opencv.hpp>

#endif

#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class Image : public Base {
        public:

            enum class ColorFormat {
                Unknow = -1,
                ARGB32,
                ARGB32NoPremultiplied,
                RGB24,
                A8,
                A1,
                RGB16_565,
                RGB30,
            };

            static ColorFormat colorFormatFromChannel(int channels) {
                switch (channels) {
                    case 4:
                        return ColorFormat::ARGB32NoPremultiplied;
                    case 3:
                        return ColorFormat::RGB24;
                    case 1:
                        return ColorFormat::A8;
                    default:
                        return ColorFormat::Unknow;
                }
            }

            Image(std::string name, double width, double height, Transform transform);

            Image(std::string name, double width, double height, Transform transform,
                  cairo_surface_t *imageSurface);

            static Image fromRaw(std::string name, double width, double height, Transform transform,
                                 unsigned char *imageData,
                                 int imageW, int imageH,
                                 int imageStride,
                                 ColorFormat colorFormat);

#ifdef OpenCV_FOUND

            static Image fromMat(std::string name, double width, double height, Transform transform,
                                 const cv::Mat &imageMat);

#endif

#ifdef OpenCV_FOUND

            static Image fromFileByCV(std::string name, double width, double height, Transform transform,
                                      const std::string &imageFilePath);

#endif

            static Image fromPNG(std::string name, double width, double height, Transform transform,
                                 const std::string &imageFilePath);

// todo webp

#ifdef JPEG_FOUND

            static Image fromJPG(std::string name, double width, double height, Transform transform,
                                 const std::string &imageFilePath);

#endif

            static Image fromFile(std::string name, double width, double height, Transform transform,
                                  const std::string &imageFilePath);


            ~Image();

        private:
            static _cairo_format toCairoFormat(ColorFormat format) {
                switch (format) {
                    case ColorFormat::ARGB32:
                        return CAIRO_FORMAT_ARGB32;
                    case ColorFormat::ARGB32NoPremultiplied:
                        return CAIRO_FORMAT_ARGB32;
                    case ColorFormat::RGB24:
                        return CAIRO_FORMAT_RGB24;
                    case ColorFormat::A8:
                        return CAIRO_FORMAT_A8;
                    case ColorFormat::A1:
                        return CAIRO_FORMAT_A1;
                    case ColorFormat::RGB16_565:
                        return CAIRO_FORMAT_RGB16_565;
                    case ColorFormat::RGB30:
                        return CAIRO_FORMAT_RGB30;
                    case ColorFormat::Unknow:
                        return CAIRO_FORMAT_INVALID;
                }
            }
        };

    } // namespace Component
} // namespace ArtRobot
