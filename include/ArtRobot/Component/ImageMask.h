#pragma once

#include "ArtRobot/Component/Base.h"
#include "ArtRobot/Component/Image.h"

namespace ArtRobot
{
namespace Component
{

class ImageMask : public Base
{
private:
public:
    ImageMask(double x, double y,
              double w, double h,
              double r,
              const string &maskImageFilePath,
              cairo_surface_t *childSurface);
    ImageMask(double x, double y,
              double w, double h,
              double r,
              const Mat &maskImageMatRead,
              cairo_surface_t *childSurface);
    ~ImageMask();
};

} // namespace Component
} // namespace ArtRobot
