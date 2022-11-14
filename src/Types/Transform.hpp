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

#include <string>
#include <cmath>
#include <cairo.h>

#include "./Property.hpp"

namespace ArtRobot {

    struct Transform {
        double x = 0;
        double y = 0;
        double rotate = 0;
        enum class Anchor {
            LT, CT, RT,
            LC, CC, RC,
            LD, CD, RD,
        } anchor = Anchor::CC;
        double scaleX = 1;
        double scaleY = 1;

        void apply(const Property &property, cairo_t *cr) const;
    };

} // namespace ArtRobot
