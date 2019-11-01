#include "default.h"

#include "ComponentRepeat.h"

namespace Render
{

ComponentRepeat::ComponentRepeat()
{
    type = ComponentTypeText;

    cairo_t *cr = cairo_create(surface);



    cairo_destroy(cr);
}

ComponentRepeat::~ComponentRepeat()
{
}

} // namespace Render
