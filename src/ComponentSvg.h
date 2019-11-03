#pragma once

#include "default.h"

#include "Component.h"

namespace Render
{

class ComponentSvg : public Component
{
private:
public:
    ComponentSvg(double x, double y,
                 double w, double h,
                 double r,
                 const string &src);
    ComponentSvg(double x, double y,
                           double w, double h,
                           double r,
                           unsigned char *data, size_t len);
    ~ComponentSvg();
};

} // namespace Render
