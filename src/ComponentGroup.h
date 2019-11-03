#pragma once

#include "default.h"

#include "Color.h"
#include "Component.h"

namespace Render
{

class ComponentGroup : public Component
{
private:
public:
    ComponentGroup();
    ~ComponentGroup();

    void addChild(cairo_surface_t *childSurface);
};

} // namespace Render
