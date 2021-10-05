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

#include "../Types/Color.hpp"
#include "./Base.hpp"

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
