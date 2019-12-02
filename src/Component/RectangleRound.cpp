#include "ArtRobot/Component/RectangleRound.h"

namespace ArtRobot
{
namespace Component
{

RectangleRound::RectangleRound(std::string __name,
                               double __x, double __y,
                               double __w, double __h,
                               double __r,
                               double angleTL,
                               double angleTR,
                               double angleBR,
                               double angleBL,
                               Color color)
    : Base(TypeRectangleRound, __name, __x, __y, __w, __h, __r)
{
    cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());

    cairo_move_to(cr, x + angleTL, y);
    cairo_line_to(cr, x + w - angleTL, y);

    cairo_move_to(cr, x + w, y + angleTR);
    cairo_line_to(cr, x + w, y + h - angleTR);

    cairo_move_to(cr, x + w - angleBR, y + h);
    cairo_line_to(cr, x + angleBR, y + h);

    cairo_move_to(cr, x, y + h - angleBL);
    cairo_line_to(cr, x, y + angleBL);

    cairo_arc(cr, x + angleTL, y + angleTL, angleTL, M_PI, 3 * M_PI / 2.0);
    cairo_arc(cr, x + w - angleTR, y + angleTR, angleTR, 3 * M_PI / 2, 2 * M_PI);
    cairo_arc(cr, x + w - angleBR, y + h - angleBR, angleBR, 0, M_PI / 2);
    cairo_arc(cr, x + angleBL, y + h - angleBL, angleBL, M_PI / 2, M_PI);

    cairo_fill(cr);
}

RectangleRound::~RectangleRound()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
