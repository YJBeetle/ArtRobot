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
    ComponentRectangle(Color color,
                       double x, double y,
                       double w, double h);
    ~ComponentRectangle();
};

} // namespace Render
