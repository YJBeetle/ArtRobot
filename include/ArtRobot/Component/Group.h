/*
 * Copyright 2020 YJBeetle
 *
 * Authors:
 *  YJBeetle <YJBeetle@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 3.  See
 * the LICENSE file in the top-level directory.
 *
 */

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