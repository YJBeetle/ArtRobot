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

#include <utility>

namespace ArtRobot {
    namespace Component {

        Repeat::Repeat(std::string name, double width, double height)
                : Repeat(std::move(name), {}, width, height) {
        }

        Repeat::Repeat(std::string name, Transform transform, double width, double height)
                : Base({name, width, height}, transform) {
        }

        Repeat::~Repeat() {
        }

        void Repeat::addChild(const Base &child) {
            const auto childWidth = child.width();
            const auto childHeight = child.height();
            if (childWidth <= 0 || childHeight <= 0 || property.width <= 0 || property.height <= 0)
                return;

            cairo_save(cr);
            cairo_rectangle(cr, 0, 0, property.width, property.height);
            cairo_clip(cr);
            for (double y = 0; y < property.height; y += childHeight) {
                for (double x = 0; x < property.width; x += childWidth) {
                    cairo_set_source_surface(cr, child.getSurface(), x, y);
                    cairo_paint(cr);
                }
            }
            cairo_restore(cr);
        }

        void Repeat::addChild(std::shared_ptr<Component::Base> child) {
            if (!child)
                return;
            addChild(*child);
            children.push_back(std::move(child));
        }

    } // namespace Component
} // namespace ArtRobot
