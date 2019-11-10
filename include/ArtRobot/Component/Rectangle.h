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
    Rectangle(std::string __name, 
              double __x, double __y,
              double __w, double __h,
              double __r,
              Color color);
    ~Rectangle();
};

} // namespace Component
} // namespace ArtRobot
