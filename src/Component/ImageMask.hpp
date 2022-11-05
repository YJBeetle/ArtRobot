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
#include <memory>

#include "./Base.hpp"
#include "./Image.hpp"

namespace ArtRobot {
    namespace Component {

        class ImageMask : public Base {
        private:
            std::shared_ptr<Image> maskImage;
            std::shared_ptr<Base> child;

        public:
#ifdef OpenCV_FOUND

            ImageMask(std::string name, double width, double height, Transform transform,
                      const std::string &maskImageFilePath,
                      std::shared_ptr<Base> child);

#endif

#ifdef OpenCV_FOUND

            ImageMask(std::string name, double width, double height, Transform transform,
                      const cv::Mat &maskImageMatRead,
                      std::shared_ptr<Base> child);

#endif

            ~ImageMask();

        };

    } // namespace Component
} // namespace ArtRobot
