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

        class Circle : public Base {
        private:
        public:
            Circle(std::string name, Transform transform,
                   double width = 10, double height = 10,
                   Color color = Color::Black);

            ~Circle();
        };

    } // namespace Component
} // namespace ArtRobot
