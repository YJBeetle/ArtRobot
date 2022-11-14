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

#include "./Repeat.hpp"

namespace ArtRobot {
    namespace Component {

        Repeat::Repeat(std::string name, double width, double height) // TODO
                : Base({name, width, height}) {
        }

        Repeat::Repeat(std::string name, double width, double height, Transform transform) // TODO
                : Base({name, width, height}, transform) {
        }

        Repeat::~Repeat() {
        }

    } // namespace Component
} // namespace ArtRobot
