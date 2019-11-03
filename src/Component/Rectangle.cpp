#include "ArtRobot/Component/Rectangle.h"

namespace ArtRobot
{
namespace Component
{

Rectangle::Rectangle(double x, double y,
                     double w, double h,
                     double r,
                     Color color)
    : Base(x, y, w, h, r)
{
    type = TypeRectangle;

    cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());
    cairo_rectangle(cr, 0, 0, w, h);
    cairo_fill(cr);
}

Rectangle::~Rectangle()
{
}

} // namespace Component
} // namespace ArtRobot
