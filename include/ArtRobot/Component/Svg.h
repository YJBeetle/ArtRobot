#pragma once

#include <string>
#include <librsvg/rsvg.h>

#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Svg : public Base
{
private:
public:
    Svg(std::string __name,
        double __x, double __y,
        double __w, double __h,
        double __r,
        const std::string &src);
    Svg(std::string __name,
        double __x, double __y,
        double __w, double __h,
        double __r,
        unsigned char *data, size_t len);
    ~Svg();
};

} // namespace Component
} // namespace ArtRobot
