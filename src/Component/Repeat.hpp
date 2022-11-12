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

#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class Repeat : public Base // TODO
        {
        private:
        public:
            Repeat(std::string name, double width, double height);

            Repeat(std::string name, double width, double height, Transform transform);

            ~Repeat();
        };

    } // namespace Component
} // namespace ArtRobot
