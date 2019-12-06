#include "ArtRobot/Component/Rectangle.h"

namespace ArtRobot
{
namespace Component
{

Rectangle::Rectangle(std::string __name,
                     double __x, double __y,
                     double __w, double __h,
                     double __r,
                     Color color)
    : Base(TypeRectangle, __name, __x, __y, __w, __h, __r)
{
    cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());
    cairo_rectangle(cr, 0, 0, _w, _h);
    cairo_fill(cr);
}

Rectangle::~Rectangle()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
