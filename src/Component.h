#pragma once

#include "default.h"

namespace Render
{

enum ComponentType
{
    ComponentTypeUnknow = 0,
    ComponentTypeRectangle,
    ComponentTypeSvg,
    ComponentTypeImage,
    ComponentTypeImageMask,
    ComponentTypeText,
    ComponentTypeRepeat,
    ComponentTypeGroup,
};

class Component
{
protected:
    ComponentType type = ComponentTypeUnknow;
    cairo_surface_t *surface = nullptr;
    cairo_t *cr = nullptr;

public:
    Component();
    Component(double x, double y,
              double w, double h,
              double r);
    ~Component();

    cairo_surface_t *getSurface();
};

} // namespace Render
