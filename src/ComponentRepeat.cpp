#include "default.h"

#include "ComponentRepeat.h"

namespace Render
{

ComponentRepeat::ComponentRepeat() // TODO
{
    type = ComponentTypeRepeat;

    cairo_t *cr = cairo_create(surface);



    cairo_destroy(cr);
}

ComponentRepeat::~ComponentRepeat()
{
}

} // namespace Render
