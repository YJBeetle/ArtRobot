#include "default.h"

#include "Component.h"

namespace Render
{

Component::Component()
{
    surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
    cr = cairo_create(surface);
}

Component::Component(double x, double y,
                     double w, double h,
                     double r)
    : Component()
{
    cairo_translate(cr, x, y);
}

Component::~Component()
{
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

cairo_surface_t *Component::getSurface()
{
    return surface;
}

} // namespace Render
