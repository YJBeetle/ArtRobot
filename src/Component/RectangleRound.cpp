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

#include "./RectangleRound.hpp"

namespace ArtRobot {
    namespace Component {

        RectangleRound::RectangleRound(std::string name, double width, double height, Transform transform,
                                       double angleTL,
                                       double angleTR,
                                       double angleBR,
                                       double angleBL,
                                       Color color)
                : Base({Property::Type::RectangleRound, name, width, height}, transform) {
            cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());

            cairo_move_to(cr, angleTL, 0);
            cairo_line_to(cr, width - angleTL, 0);

            cairo_move_to(cr, width, angleTR);
            cairo_line_to(cr, width, height - angleTR);

            cairo_move_to(cr, width - angleBR, height);
            cairo_line_to(cr, angleBR, height);

            cairo_move_to(cr, 0, height - angleBL);
            cairo_line_to(cr, 0, angleBL);

            cairo_arc(cr, angleTL, angleTL, angleTL, M_PI, 3 * M_PI / 2.0);
            cairo_arc(cr, width - angleTR, angleTR, angleTR, 3 * M_PI / 2, 2 * M_PI);
            cairo_arc(cr, width - angleBR, height - angleBR, angleBR, 0, M_PI / 2);
            cairo_arc(cr, angleBL, height - angleBL, angleBL, M_PI / 2, M_PI);

            cairo_fill(cr);
        }

        RectangleRound::~RectangleRound() {
            finish();
        }

    } // namespace Component
} // namespace ArtRobot
