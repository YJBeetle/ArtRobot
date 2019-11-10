#pragma once

#include <memory>
#include <vector>

#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Group : public Base
{
private:
    std::vector<std::shared_ptr<Base>> childs;

public:
    Group(std::string __name);
    ~Group();

    void addChild(std::shared_ptr<Base> child);
};

} // namespace Component
} // namespace ArtRobot