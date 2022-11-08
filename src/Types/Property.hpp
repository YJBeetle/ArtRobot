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

    struct Property {
        enum class Type {
            Unknow = 0,
            Rectangle,      // 方形
            RectangleRound, // 圆角方形
            Circle,         // 圆形
            Svg,
            Image,
            Mask,
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

} // namespace ArtRobot
