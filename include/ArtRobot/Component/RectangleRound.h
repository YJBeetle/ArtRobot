#pragma once

#include "ArtRobot/Types/Color.h"
#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class RectangleRound : public Base
{
private:
public:
    RectangleRound(std::string __name,
                   double __x, double __y,
                   double __w, double __h,
                   double __r,
                   double angleTL,
                   double angleTR,
                   double angleBR,
                   double angleBL,
                   Color color);
    ~RectangleRound();
};

} // namespace Component
} // namespace ArtRobot
