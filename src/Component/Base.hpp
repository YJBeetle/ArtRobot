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

#include <string>
#include <cmath>
#include <cairo.h>

namespace ArtRobot {
    namespace Component {

        struct Property {
            enum class Type {
                Unknow = 0,
                Rectangle,      // 方形
                RectangleRound, // 圆角方形
                Circle,         // 圆形
                Svg,
                Image,
                ImageMask,
                Text,     // 点状文字
                TextArea, // 区域文字
                Repeat,   // 重复对象
                Group,    // 群组对象
            };
            Type type = Type::Unknow;
            std::string name = std::string();
            double width = 0;
            double height = 0;
        };
        struct Transform {
            double x = 0;
            double y = 0;
            double rotate = 0;
            enum class Anchor {
                LT, CT, RT,
                LC, CC, RC,
                LD, CD, RD,
            };
            Anchor anchor = Anchor::CC;
            double scaleX = 1;
            double scaleY = 1;
        };

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

            void finish();

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
