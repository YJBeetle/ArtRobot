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
    ComponentImageMask(const string &imageFilePath,
                       double x, double y,
                       double w, double h,
                       cairo_surface_t *childSurface);
    ComponentImageMask(const Mat &image,
                       double x, double y,
                       double w, double h,
                       cairo_surface_t *childSurface);
    ~ComponentImageMask();
};

} // namespace Render
