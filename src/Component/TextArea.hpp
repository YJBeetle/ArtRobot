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
#include "../Types/TextAlign.hpp"
#include "./Base.hpp"

namespace ArtRobot {
    namespace Component {

        class TextArea : public Base {
        private:
            double _realW;
            double _realH;

        public:
            TextArea(std::string name, Transform transform,
                     double width, double height,
                     const std::string &content,                     // 内容
                     Color color = Color::Black,                     // 颜色
                     const std::string &fontFamily = std::string(),  // 字体
                     int fontWeight = 500,                           // 粗细
                     double fontSize = 10.,                          // 字号
                     HorizontalAlign hAlign = HorizontalAlign::Left, // 水平对齐方式
                     VerticalAlign vAlign = VerticalAlign::Top,      // 垂直对齐方式
                     double lineSpacing = 0,                         // 行间距
                     double wordSpacing = 0);                        // 字间距
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
