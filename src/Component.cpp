#include "default.h"

#include "Component.h"

namespace Render
{

Component::Component() : type(ComponentTypeUnknow),
                         surface(nullptr)

{
    surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
}

Component::~Component()
{
    cairo_surface_destroy(surface);
}

cairo_surface_t *Component::getSurface()
{
    return surface;
}

} // namespace Render
