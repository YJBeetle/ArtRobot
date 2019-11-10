#pragma once

#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Repeat : public Base // TODO
{
private:
public:
    Repeat(std::string __name, 
           double __x, double __y,
           double __w, double __h,
           double __r);
    ~Repeat();
};

} // namespace Component
} // namespace ArtRobot
