#pragma once

#include "default.h"

#include "Color.h"
#include "Component.h"

namespace Render
{

class ComponentRectangle : public Component
{
private:
public:
    ComponentRectangle(double x, double y,
                       double w, double h,
                       double r,
                       Color color);
    ~ComponentRectangle();
};

} // namespace Render
