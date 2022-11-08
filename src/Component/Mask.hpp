/*
 * Copyright 2020 YJBeetle
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

#include "./Base.hpp"
#include "./Image.hpp"

namespace ArtRobot {
    namespace Component {

        class Mask : public Base {
        public:
            Mask(std::string name, double width, double height, Transform transform,
                      const Base &mask,
                      const Base &child);

#ifdef OpenCV_FOUND

            static Mask fromCvMat(std::string name, double width, double height, Transform transform,
                                       const cv::Mat &maskImageMatRead,
                                       Base child);

#endif

#ifdef OpenCV_FOUND
#ifndef OpenCV_WITHOUT_IMAPI

            static Mask fromFile(std::string name, double width, double height, Transform transform,
                                      const std::string &maskImageFilePath,
                                      Base child);

#endif
#endif

            ~Mask();

        };

    } // namespace Component
} // namespace ArtRobot
