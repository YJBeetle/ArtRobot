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

#include <cstdint>

namespace ArtRobot {

    class Color {
    private:
        uint32_t value;

    public:
        Color();

        Color(uint32_t colorVal);

        Color(const char *colorStr);

        Color &operator=(uint32_t colorVal);

        Color &operator=(const char *colorStr);

        Color &operator=(const Color &other);

        double red();

        double green();

        double blue();

        double alpha();
    };

} // namespace ArtRobot
