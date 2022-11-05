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

#include "./Group.hpp"

#include <iostream>

namespace ArtRobot {
    namespace Component {

        Group::Group(std::string name, Transform transform)
                : Base({Property::Type::Group, name, 0, 0}, transform) {
        }

        Group::~Group() {
            finish();
        }

        void Group::addChild(const Base &child) {
            cairo_save(cr); //保存画笔
            cairo_set_source_surface(cr, child.getSurface(), 0.0, 0.0);
            cairo_paint(cr);
            cairo_restore(cr); //还原画笔
        }

    } // namespace Component
} // namespace ArtRobot
