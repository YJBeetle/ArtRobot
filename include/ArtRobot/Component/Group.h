#pragma once

#include <vector>

using namespace std;

#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Group : public Base
{
private:
    vector<shared_ptr<Base>> childs;

public:
    Group();
    ~Group();

    void addChild(shared_ptr<Base> child);
};

} // namespace Component
} // namespace ArtRobot