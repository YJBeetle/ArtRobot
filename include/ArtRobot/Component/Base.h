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
    TypeRectangle,      // 方形
    TypeRectangleRound, // 圆角方形
    TypeCircle,         // 圆形
    TypeSvg,
    TypeImage,
    TypeImageMask,
    TypeText,     // 点状文字
    TypeTextArea, // 区域文字
    TypeRepeat,   // 重复对象
    TypeGroup,    // 群组对象
};

class Base
{
protected:
    Type type = TypeUnknow;
    std::string name;
    double _x = 0;
    double _y = 0;
    double _w = 0;
    double _h = 0;
    double _r = 0;
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

    inline void setX(double __x)
    {
        _x = __x;
    }
    inline void setY(double __y)
    {
        _y = __y;
    }
    inline double x()
    {
        return _x;
    }
    inline double y()
    {
        return _y;
    }
    inline double w()
    {
        return _w;
    }
    inline double h()
    {
        return _h;
    }
    inline double r()
    {
        return _r;
    }
};

} // namespace Component
} // namespace ArtRobot
