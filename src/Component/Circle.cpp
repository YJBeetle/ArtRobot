#include "ArtRobot/Component/Circle.h"

namespace ArtRobot
{
namespace Component
{

Circle::Circle(std::string __name,
               double __x, double __y,
               double __w, double __h,
               double __r,
               Color color)
    : Base(TypeRectangleRound, __name, __x, __y, __w, __h, __r)
{
    cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());

    cairo_scale(cr,__w / 2.0, __h / 2.0);
    cairo_arc(cr, 1, 1, 1.0, 0.0, 2 * M_PI);

    cairo_fill(cr);
}

Circle::~Circle()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
