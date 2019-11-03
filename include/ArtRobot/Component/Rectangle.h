#pragma once

#include "ArtRobot/Types/Color.h"
#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Rectangle : public Base
{
private:
public:
    Rectangle(double x, double y,
              double w, double h,
              double r,
              Color color);
    ~Rectangle();
};

} // namespace Component
} // namespace ArtRobot
