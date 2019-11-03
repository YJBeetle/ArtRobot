#pragma once

#include <cmath>
#include <cairo.h>

namespace ArtRobot
{
namespace Component
{

enum Type
{
    TypeUnknow = 0,
    TypeRectangle,
    TypeSvg,
    TypeImage,
    TypeImageMask,
    TypeText,
    TypeRepeat,
    TypeGroup,
};

class Base
{
protected:
    Type type = TypeUnknow;
    cairo_surface_t *surface = nullptr;
    cairo_t *cr = nullptr;

public:
    Base();
    Base(double x, double y,
         double w, double h,
         double r);
    ~Base();

    cairo_surface_t *getSurface();
};

} // namespace Component
} // namespace ArtRobot
