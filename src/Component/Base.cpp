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

        Base::Base(Property property)
                : property(property) {
            if (property.type != Property::Type::Unknow) {
                surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, nullptr);
                cr = cairo_create(surface);
            }
        }

        Base::Base(Property property, Transform transform)
                : property(property),
                  transform(transform) {
            if (property.type != Property::Type::Unknow) {
                surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, nullptr);
                cr = cairo_create(surface);
                cairo_translate(cr, property.width / 2, property.height / 2);
                cairo_rotate(cr, transform.rotate * M_PI / 180);
                cairo_translate(cr, -property.width / 2, -property.height / 2);
            }
        }

        Base::~Base() {
            if (cr)
                cairo_destroy(cr);
            if (surface)
                cairo_surface_destroy(surface);
        }

        void Base::finish() {
            if (cr)
                cairo_destroy(cr);
            cr = nullptr;
            // cairo_surface_finish(surface);
        }

        cairo_surface_t *Base::getSurface() {
            return (property.type != Property::Type::Unknow)
                   ? surface
                   : nullptr;
        }

    } // namespace Component
} // namespace ArtRobot
