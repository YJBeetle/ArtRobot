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
        uint32_t color;

    public:
        Color();

        Color(uint32_t __color);

        Color(const char *__color);

        Color &operator=(uint32_t __color);

        Color &operator=(const char *__color);

        Color &operator=(const Color &__color);

        double red();

        double green();

        double blue();

        double alpha();
    };

} // namespace ArtRobot
