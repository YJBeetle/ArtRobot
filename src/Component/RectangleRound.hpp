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

namespace ArtRobot {
    namespace Component {

        class RectangleRound : public Base {
        private:
        public:
            RectangleRound(std::string name, double width, double height, Transform transform,
                           double angleTL,
                           double angleTR,
                           double angleBR,
                           double angleBL,
                           Color color);

            ~RectangleRound();
        };

    } // namespace Component
} // namespace ArtRobot
