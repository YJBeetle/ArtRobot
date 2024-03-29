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

namespace ArtRobot {

    struct Property {
        std::string name = std::string();
        double width = 0;
        double height = 0;
    };

} // namespace ArtRobot
