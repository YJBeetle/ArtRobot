#pragma once

#include <string>
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
    TypeRectangleRound,
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
    std::string name;
    double x = 0;
    double y = 0;
    double w = 100;
    double h = 100;
    double r = 0;
    cairo_surface_t *surface = nullptr;
    cairo_t *cr = nullptr;

public:
    Base();
    Base(Type __type, std::string __name);
    Base(Type __type, std::string __name,
         double __x, double __y,
         double __w, double __h,
         double __r);
    virtual ~Base();

    void finish();
    cairo_surface_t *getSurface();
};

} // namespace Component
} // namespace ArtRobot
