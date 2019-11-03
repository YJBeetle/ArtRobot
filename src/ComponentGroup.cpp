#include "default.h"

#include "ComponentGroup.h"

namespace Render
{

ComponentGroup::ComponentGroup()
    : Component()
{
    type = ComponentTypeGroup;
}

ComponentGroup::~ComponentGroup()
{
}

void ComponentGroup::addChild(cairo_surface_t *childSurface)
{
    cairo_save(cr); //保存画笔
    cairo_set_source_surface(cr, childSurface, 0.0, 0.0);
    cairo_paint(cr);
    cairo_restore(cr); //还原画笔
}

} // namespace Render
