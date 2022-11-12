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

#include "./Circle.hpp"

namespace ArtRobot {
    namespace Component {

        Circle::Circle(std::string name, Transform transform, double width, double height, Color color)
                : Base({Property::Type::Circle, name, width, height}, transform) {
            cairo_set_source_rgba(cr, color.r(), color.g(), color.b(), color.a());

            cairo_scale(cr, width / 2.0, height / 2.0);
            cairo_arc(cr, 1., 1., 1., 0., 2. * M_PI);

            cairo_fill(cr);
        }

        Circle::~Circle() {
        }

    } // namespace Component
} // namespace ArtRobot
