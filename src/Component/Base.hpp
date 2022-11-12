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

#pragma once
#include <ArtRobot/Features.hpp>

#include <string>
#include <cmath>
#include <cairo.h>

#include "../Types/Property.hpp"
#include "../Types/Transform.hpp"

namespace ArtRobot {
    namespace Component {

        class Base {
        protected:
            Property property;
            Transform transform;
            cairo_surface_t *surface = nullptr;
            cairo_t *cr = nullptr;

        public:
            Base();

            Base(Property property);

            Base(Property property, Transform transform);

            virtual ~Base();

            cairo_surface_t *getSurface() const;

            inline double width() const {
                return property.width;
            }

            inline double height() const {
                return property.height;
            }

            inline void setX(double x) {
                transform.x = x;
            }

            inline void setY(double y) {
                transform.y = y;
            }

            inline void setRotate(double rotate) {
                transform.rotate = rotate;
            }

            inline double x() const {
                return transform.x;
            }

            inline double y() const {
                return transform.y;
            }

            inline double rotate() const {
                return transform.rotate;
            }
        };

    } // namespace Component
} // namespace ArtRobot
