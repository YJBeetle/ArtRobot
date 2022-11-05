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

#include "./Rectangle.hpp"

namespace ArtRobot {
    namespace Component {

        Rectangle::Rectangle(std::string name, double width, double height, Transform transform, Color color)
                : Base({Property::Type::Rectangle, name, width, height}, transform) {
            cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());
            cairo_rectangle(cr, 0, 0, width, height);
            cairo_fill(cr);
        }

        Rectangle::~Rectangle() {
            finish();
        }

    } // namespace Component
} // namespace ArtRobot
