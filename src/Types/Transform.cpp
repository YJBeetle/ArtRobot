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

#include "./Transform.hpp"

namespace ArtRobot {

    void Transform::apply(const Property &property, cairo_t *cr) const {
        cairo_translate(cr, x, y);
        cairo_rotate(cr, rotate * M_PI / 180);
        cairo_scale(cr, scaleX, scaleY);
        switch (anchor) {
            case Transform::Anchor::LT:
                break;
            case Transform::Anchor::CT:
                cairo_translate(cr, -property.width / 2, 0);
                break;
            case Transform::Anchor::RT:
                cairo_translate(cr, -property.width, 0);
                break;
            case Transform::Anchor::LC:
                cairo_translate(cr, 0, -property.height / 2);
                break;
            case Transform::Anchor::CC:
                cairo_translate(cr, -property.width / 2, -property.height / 2);
                break;
            case Transform::Anchor::RC:
                cairo_translate(cr, -property.width, -property.height / 2);
                break;
            case Transform::Anchor::LD:
                cairo_translate(cr, 0, -property.height);
                break;
            case Transform::Anchor::CD:
                cairo_translate(cr, -property.width / 2, -property.height);
                break;
            case Transform::Anchor::RD:
                cairo_translate(cr, -property.width, -property.height);
                break;
        }
    }

} // namespace ArtRobot
