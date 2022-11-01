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

namespace ArtRobot
{
namespace Component
{

class ImageMask : public Base
{
private:
    std::shared_ptr<Image> maskImage;
    std::shared_ptr<Base> child;

    void drawChildSurfaceUsedMask(const cv::Mat &maskImageMatRead);

public:
#ifndef WASM
    ImageMask(std::string __name,
              double __x, double __y,
              double __w, double __h,
              double __r,
              const std::string &maskImageFilePath,
              std::shared_ptr<Base> child);
#endif
    ImageMask(std::string __name,
              double __x, double __y,
              double __w, double __h,
              double __r,
              const cv::Mat &maskImageMatRead,
              std::shared_ptr<Base> child);
    ~ImageMask();
};

} // namespace Component
} // namespace ArtRobot
