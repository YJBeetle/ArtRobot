#include "default.h"

#include "ComponentRectangle.h"

namespace Render
{

ComponentRectangle::ComponentRectangle(Color color,
                                       double x, double y,
                                       double w, double h)
{
    type = ComponentTypeRectangle;

    cairo_t *cr = cairo_create(surface);
    cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());
    cairo_rectangle(cr, x, y, w, h);
    cairo_fill(cr);
    cairo_destroy(cr);
}

ComponentRectangle::~ComponentRectangle()
{
}

} // namespace Render
