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

    cairo_move_to(cr, angleTL, 0);
    cairo_line_to(cr, w - angleTL, 0);

    cairo_move_to(cr, w, angleTR);
    cairo_line_to(cr, w, h - angleTR);

    cairo_move_to(cr, w - angleBR, h);
    cairo_line_to(cr, angleBR, h);

    cairo_move_to(cr, 0, h - angleBL);
    cairo_line_to(cr, 0, angleBL);

    cairo_arc(cr, angleTL, angleTL, angleTL, M_PI, 3 * M_PI / 2.0);
    cairo_arc(cr, w - angleTR, angleTR, angleTR, 3 * M_PI / 2, 2 * M_PI);
    cairo_arc(cr, w - angleBR, h - angleBR, angleBR, 0, M_PI / 2);
    cairo_arc(cr, angleBL, h - angleBL, angleBL, M_PI / 2, M_PI);

    cairo_fill(cr);
}

RectangleRound::~RectangleRound()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
