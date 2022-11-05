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
#include <tuple>
#include <pango/pangocairo.h>

#include "../Types/Color.hpp"
#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class TextArea : public Base {
        private:
            double _realW;
            double _realH;

        public:
            TextArea(std::string name, double width, double height, Transform transform,
                     const std::string &content,    // 内容
                     Color color,                   // 颜色
                     const std::string &fontFamily, // 字体
                     int fontWeight,                // 粗细
                     double fontSize,               // 字号
                     int8_t horizontalAlign,        // 水平对齐方式，0为左对齐，1居中，2右对齐
                     int8_t verticalAlign,          // 垂直对齐方式，0为第一行基线对齐，1为顶部对齐，2垂直居中对齐，3底部对齐
                     double lineSpacing = 0,        // 行间距
                     double wordSpacing = 0);       // 字间距
            ~TextArea();

            inline std::tuple<double, double> getRealSize() const {
                return {_realW, _realH};
            }

            inline double realW() const {
                return _realW;
            }

            inline double realH() const {
                return _realH;
            }
        };

    } // namespace Component
} // namespace ArtRobot
