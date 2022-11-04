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

        Rectangle::Rectangle(std::string __name,
                             double __x, double __y,
                             double __w, double __h,
                             double __r,
                             Color color)
                : Base(TypeRectangle, __name, __x, __y, __w, __h, __r) {
            cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());
            cairo_rectangle(cr, 0, 0, _w, _h);
            cairo_fill(cr);
        }

        Rectangle::~Rectangle() {
            finish();
        }

    } // namespace Component
} // namespace ArtRobot
