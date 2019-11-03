#pragma once


#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Group : public Base
{
private:
public:
    Group();
    ~Group();

    void addChild(cairo_surface_t *childSurface);
};

} // namespace Component
} // namespace ArtRobot