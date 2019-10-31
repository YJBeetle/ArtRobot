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

#include "ArtRobot/Component/Repeat.h"

namespace ArtRobot
{
namespace Component
{

Repeat::Repeat(std::string __name,
               double __x, double __y,
               double __w, double __h,
               double __r) // TODO
    : Base(TypeRepeat, __name, __x, __y, __w, __h, __r)
{
}

Repeat::~Repeat()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
