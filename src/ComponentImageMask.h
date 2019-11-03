#pragma once

#include "default.h"

#include "Color.h"
#include "Component.h"

namespace Render
{

class ComponentImageMask : public Component
{
private:
public:
    ComponentImageMask(double x, double y,
                       double w, double h,
                       double r,
                       const string &maskImageFilePath,
                       cairo_surface_t *childSurface);
    ComponentImageMask(double x, double y,
                       double w, double h,
                       double r,
                       const Mat &maskImageMatRead,
                       cairo_surface_t *childSurface);
    ~ComponentImageMask();
};

} // namespace Render
