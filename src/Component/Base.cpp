#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

Base::Base()
{
    surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
    cr = cairo_create(surface);
}

Base::Base(double x, double y,
           double w, double h,
           double r)
    : Base()
{
    cairo_translate(cr, x, y);
    cairo_translate(cr, w / 2, h / 2);
    cairo_rotate(cr, r * M_PI / 180);
    cairo_translate(cr, -w / 2, -h / 2);
}

Base::~Base()
{
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

cairo_surface_t *Base::getSurface()
{
    return surface;
}

} // namespace Component
} // namespace ArtRobot
