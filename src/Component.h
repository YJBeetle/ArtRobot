#pragma once

#include "default.h"

namespace Render
{

enum ComponentType
{
    ComponentTypeUnknow = 0,
    ComponentTypeRectangle,
    ComponentTypeImage,
    ComponentTypeImageMask,
    ComponentTypeText,
    ComponentTypeRepeat,
    ComponentTypeGroup,
};

class Component
{
protected:
    ComponentType type;
    cairo_surface_t *surface;

public:
    Component();
    ~Component();

    cairo_surface_t *getSurface();
};

} // namespace Render
