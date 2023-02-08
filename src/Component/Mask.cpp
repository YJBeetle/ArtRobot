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

#include "./Mask.hpp"

namespace ArtRobot {
    namespace Component {

        Mask::Mask(std::string name, double width, double height, Transform transform,
                   const Base &mask,
                   const Base &child)
                : Base({name, width, height}, transform) {
            cairo_set_source_surface(cr, child.getSurface(), 0.0, 0.0);
            cairo_mask_surface(cr, mask.getSurface(), 0, 0);
            cairo_fill(cr);
        }

        Mask::Mask(std::string name, double width, double height, Transform transform,
                   std::shared_ptr<Component::Base> mask, const Base &child)
                : Mask(name, width, height, transform, *mask, child) {
            mask = mask;
        }

        Mask::Mask(std::string name, double width, double height, Transform transform,
                   const Base &mask, std::shared_ptr<Component::Base> child)
                : Mask(name, width, height, transform, mask, *child) {
            child = child;
        }

        Mask::Mask(std::string name, double width, double height, Transform transform,
                   std::shared_ptr<Component::Base> mask, std::shared_ptr<Component::Base> child)
                : Mask(name, width, height, transform, *mask, *child) {
            mask = mask;
            child = child;
        }

#ifdef OpenCV_FOUND

        Mask Mask::fromCvMat(std::string name, double width, double height, Transform transform,
                             const cv::Mat &maskImageMatRead,
                             Base __child) {
            cv::Mat maskImageMat;
            if (maskImageMatRead.channels() == 3)
                cvtColor(maskImageMatRead, maskImageMat, cv::COLOR_BGR2BGRA);
            else
                maskImageMat = maskImageMatRead;

            // 解决Ai的蒙版兼容性问题，Ai的蒙版使用的是黑白，而Acrobat中使用的是Alpha。
            for (int y = 0; y < maskImageMat.rows; y++)
                for (int x = 0; x < maskImageMat.cols; x++) {
                    auto &p = maskImageMat.at<cv::Vec4b>(y, x);
                    uchar a = p[3];
                    uchar b = (p[2] * 30 + p[1] * 59 + p[0] * 11) / 100;
                    p[0] = p[1] = p[2] = p[3] = MIN(a, b);
                }

            return Mask(name, width, height, transform, Image(name, transform, maskImageMat, width, height), __child);
        }

#endif

#ifdef OpenCV_FOUND
#ifndef OpenCV_WITHOUT_IMAPI

        Mask Mask::fromFile(std::string name, double width, double height, Transform transform,
                            const std::string &maskImageFilePath,
                            Base __child) {
            return Mask::fromCvMat(name, width, height, transform, cv::imread(maskImageFilePath, cv::IMREAD_UNCHANGED), __child);
        }

#endif
#endif

        Mask::~Mask() {
        }

    } // namespace Component
} // namespace ArtRobot
