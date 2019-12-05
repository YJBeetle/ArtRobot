#pragma once

#include "ArtRobot/Types/Color.h"
#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Circle : public Base
{
private:
public:
    Circle(std::string __name,
           double __x, double __y,
           double __w, double __h,
           double __r,
           Color color);
    ~Circle();
};

} // namespace Component
} // namespace ArtRobot
