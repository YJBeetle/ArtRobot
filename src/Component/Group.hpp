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

#include <memory>
#include <vector>

#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class Group : public Base {
        public:
            Group(std::string name);

            ~Group();

            void addChild(const Base &child);
        };

    } // namespace Component
} // namespace ArtRobot