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

#include <ArtRobot/Features.hpp>

#include <string>

#ifdef OpenCV_FOUND

#include <opencv2/opencv.hpp>

#endif

#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class Image : public Base {
        public:

            Image(std::string name);

            Image(std::string name, Transform transform,
                  unsigned char *imageData,
                  int imageColums, int imageRows,
                  int imageStride, bool isPremultiplied = true,
                  double width = NAN, double height = NAN);

            Image(std::string name, Transform transform,
                  const std::vector<uint8_t> &data,
                  double width = NAN, double height = NAN);

            Image(std::string name, Transform transform,
                  const std::string &imageFilePath,
                  double width = NAN, double height = NAN);

#ifdef OpenCV_FOUND

            // 仅 BGRA
            Image(std::string name, Transform transform,
                  const cv::Mat &imageMat,
                  double width = NAN, double height = NAN);

#endif

            ~Image();

        private:
            cairo_surface_t *imageSurface = nullptr;
            int imageCols = 0;
            int imageRows = 0;

            Image(std::string name, Transform transform,
                  cairo_surface_t *imageSurface,
                  double width = NAN, double height = NAN);

            // 因为 cairo 的特性 只支持 BGRA 排列
            static cairo_surface_t *surfaceFromRaw(unsigned char *imageData,
                                                   int imageColums, int imageRows,
                                                   int imageStride, bool isPremultiplied = true);

            static cairo_surface_t *surfaceFromPng(const std::vector<uint8_t> &data);

            static cairo_surface_t *surfaceFromPng(const std::string &filename);

// todo webp

#ifdef JPEG_FOUND

            static cairo_surface_t *surfaceFromJpg(const std::vector<uint8_t> &data);

            static cairo_surface_t *surfaceFromJpg(const std::string &filename);

#endif

            static cairo_surface_t *surfaceFromFile(const std::vector<uint8_t> &data);

            static cairo_surface_t *surfaceFromFile(const std::string &filename);

        };

    } // namespace Component
} // namespace ArtRobot
