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

#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        Base::Base() {
        }

        Base::Base(Property property, Transform transform)
                : property(property),
                  transform(transform) {
            surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, nullptr);
            cr = cairo_create(surface);
            transform.apply(property, cr);
        }

        Base::~Base() {
            if (cr)
                cairo_destroy(cr);
            if (surface)
                cairo_surface_destroy(surface);
        }

    } // namespace Component
} // namespace ArtRobot
