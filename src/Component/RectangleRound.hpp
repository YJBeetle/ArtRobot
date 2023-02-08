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

#include "../Types/Color.hpp"
#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class RectangleRound : public Base {
        private:
        public:
            RectangleRound(std::string name, Transform transform,
                           double width = 10, double height = 10,
                           double angleTL = 2, double angleTR = 2, double angleBR = 2, double angleBL = 2,
                           Color color = Color::Black);

            ~RectangleRound();
        };

    } // namespace Component
} // namespace ArtRobot
