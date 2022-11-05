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

#include "./ImageMask.hpp"

namespace ArtRobot {
    namespace Component {

#ifdef OpenCV_FOUND

        ImageMask::ImageMask(std::string name, double width, double height, Transform transform,
                             const std::string &maskImageFilePath,
                             std::shared_ptr<Base> __child)
                : ImageMask(name, width, height, transform, cv::imread(maskImageFilePath, cv::IMREAD_UNCHANGED), __child) {
        }

#endif

#ifdef OpenCV_FOUND

        ImageMask::ImageMask(std::string name, double width, double height, Transform transform,
                             const cv::Mat &maskImageMatRead,
                             std::shared_ptr<Base> __child)
                : Base({Property::Type::ImageMask, name, width, height}, transform) {
            child = __child;

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

            maskImage = Image::fromMat(name, width, height, transform, maskImageMat);

            cairo_set_source_surface(cr, child->getSurface(), 0.0, 0.0);
            cairo_mask_surface(cr, maskImage->getSurface(), 0, 0);
            cairo_fill(cr);
        }

#endif

        ImageMask::~ImageMask() {
            finish();
        }

    } // namespace Component
} // namespace ArtRobot
