#include "default.h"

#include "ComponentImageMask.h"

namespace Render
{

ComponentImageMask::ComponentImageMask()
{
    type = ComponentTypeText;

    cairo_t *cr = cairo_create(surface);



    cairo_destroy(cr);
}

ComponentImageMask::~ComponentImageMask()
{
}

} // namespace Render
