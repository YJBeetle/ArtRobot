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

#include <ArtRobot/Features.hpp>

#ifdef RSVG_FOUND

#include <string>
#include <librsvg/rsvg.h>

#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class Svg : public Base {
        private:
        public:
            Svg(std::string name, Transform transform,
                double width, double height,
                const std::string &src);

            Svg(std::string name, Transform transform,
                double width, double height,
                unsigned char *data, size_t len);

            ~Svg();
        };

    } // namespace Component
} // namespace ArtRobot

#endif
