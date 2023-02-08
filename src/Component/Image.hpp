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

            // 因为 cairo 的特性 只支持 BGRA 排列
            static Image fromRaw(std::string name, Transform transform,
                                 unsigned char *imageData,
                                 int imageColums, int imageRows,
                                 int imageStride, bool isPremultiplied = true,
                                 double width = NAN, double height = NAN);

#ifdef OpenCV_FOUND

            static Image fromMat(std::string name, Transform transform,
                                 const cv::Mat &imageMat,
                                 double width = NAN, double height = NAN);

#endif

#ifdef OpenCV_FOUND
#ifndef OpenCV_WITHOUT_IMAPI

            static Image fromFileByCV(std::string name, Transform transform,
                                      const std::string &imageFilePath,
                                      double width = NAN, double height = NAN);

#endif
#endif

            static Image fromPng(std::string name, Transform transform,
                                 const std::vector<uint8_t> &data,
                                 double width = NAN, double height = NAN);

            static Image fromPng(std::string name, Transform transform,
                                 const std::string &imageFilePath,
                                 double width = NAN, double height = NAN);

// todo webp

#ifdef JPEG_FOUND

            static Image fromJpg(std::string name, Transform transform,
                                 const std::vector<uint8_t> &data,
                                 double width = NAN, double height = NAN);

            static Image fromJpg(std::string name, Transform transform,
                                 const std::string &filename,
                                 double width = NAN, double height = NAN);

#endif

            static Image fromFile(std::string name, Transform transform,
                                  const std::vector<uint8_t> &data,
                                  double width = NAN, double height = NAN);

            static Image fromFile(std::string name, Transform transform,
                                  const std::string &imageFilePath,
                                  double width = NAN, double height = NAN);


            ~Image();

        private:
            cairo_surface_t *imageSurface = nullptr;
            int imageCols = 0;
            int imageRows = 0;

            Image(std::string name, Transform transform,
                  cairo_surface_t *imageSurface,
                  double width = NAN, double height = NAN);

        };

    } // namespace Component
} // namespace ArtRobot
