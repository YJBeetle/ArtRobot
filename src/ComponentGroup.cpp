#include "default.h"

#include "ComponentGroup.h"

namespace Render
{

ComponentGroup::ComponentGroup()
{
    type = ComponentTypeGroup;

    cr = cairo_create(surface);
}

ComponentGroup::~ComponentGroup()
{
    cairo_destroy(cr);
}

void ComponentGroup::addChild(cairo_surface_t *childSurface)
{
    cairo_set_source_surface(cr, childSurface, 0.0, 0.0);
    cairo_paint(cr);
}

} // namespace Render
