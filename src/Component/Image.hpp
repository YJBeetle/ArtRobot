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

            Image(std::string name);

            static Image fromRaw(std::string name, Transform transform,
                                 unsigned char *imageData,
                                 int imageColums, int imageRows,
                                 int imageStride,
                                 ColorFormat colorFormat,
                                 double width = 0., double height = 0.);

#ifdef OpenCV_FOUND

            static Image fromMat(std::string name, Transform transform,
                                 const cv::Mat &imageMat,
                                 double width = 0., double height = 0.);

#endif

#ifdef OpenCV_FOUND

            static Image fromFileByCV(std::string name, Transform transform,
                                      const std::string &imageFilePath,
                                      double width = 0., double height = 0.);

#endif

            static Image fromPng(std::string name, Transform transform,
                                 const std::string &imageFilePath,
                                 double width = 0., double height = 0.);

// todo webp

#ifdef JPEG_FOUND

            static Image fromJpg(std::string name, Transform transform,
                                 const std::string &filename,
                                 double width = 0., double height = 0.);

#endif

            static Image fromFile(std::string name, Transform transform,
                                  const std::string &imageFilePath,
                                  double width = 0., double height = 0.);


            ~Image();

        private:
            cairo_surface_t *imageSurface;

            Image(std::string name, Transform transform,
                  cairo_surface_t *imageSurface,
                  double width = 0., double height = 0.);

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
