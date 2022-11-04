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

#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class Repeat : public Base // TODO
        {
        private:
        public:
            Repeat(std::string __name,
                   double __x, double __y,
                   double __w, double __h,
                   double __r);

            ~Repeat();
        };

    } // namespace Component
} // namespace ArtRobot
