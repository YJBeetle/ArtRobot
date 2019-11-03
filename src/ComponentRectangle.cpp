#include "default.h"

#include "ComponentRectangle.h"

namespace Render
{

ComponentRectangle::ComponentRectangle(double x, double y,
                                       double w, double h,
                                       double r,
                                       Color color)
    : Component(x, y, w, h, r)
{
    type = ComponentTypeRectangle;

    cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());
    cairo_rectangle(cr, 0, 0, w, h);
    cairo_fill(cr);
}

ComponentRectangle::~ComponentRectangle()
{
}

} // namespace Render
