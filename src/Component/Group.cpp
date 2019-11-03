#include "ArtRobot/Component/Group.h"

namespace ArtRobot
{
namespace Component
{

Group::Group()
{
    type = TypeGroup;
}

Group::~Group()
{
}

void Group::addChild(cairo_surface_t *childSurface)
{
    cairo_save(cr); //保存画笔
    cairo_set_source_surface(cr, childSurface, 0.0, 0.0);
    cairo_paint(cr);
    cairo_restore(cr); //还原画笔
}

} // namespace Component
} // namespace ArtRobot
