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

#include <memory>

#include "ArtRobot/Component/Base.h"
#include "ArtRobot/Component/Image.h"

namespace ArtRobot
{
namespace Component
{

class ImageMask : public Base
{
private:
    Image *maskImage = nullptr;
    std::shared_ptr<Base> child;

    void drawChildSurfaceUsedMask(const cv::Mat &maskImageMatRead);

public:
    ImageMask(std::string __name,
              double __x, double __y,
              double __w, double __h,
              double __r,
              const std::string &maskImageFilePath,
              std::shared_ptr<Base> child);
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
