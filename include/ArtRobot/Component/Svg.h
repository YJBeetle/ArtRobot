#pragma once

#include <string>
#include <librsvg/rsvg.h>

using namespace std;

#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Svg : public Base
{
private:
public:
    Svg(double x, double y,
        double w, double h,
        double r,
        const string &src);
    Svg(double x, double y,
        double w, double h,
        double r,
        unsigned char *data, size_t len);
    ~Svg();
};

} // namespace Component
} // namespace ArtRobot
