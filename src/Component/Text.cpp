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

#include "./Text.hpp"

namespace ArtRobot {
    namespace Component {

        Text::Text(std::string name, Transform transform,
                   const std::string &content,    // 内容
                   Color color,                   // 颜色
                   const std::string &fontFamily, // 字体
                   int fontWeight,                // 粗细
                   double fontSize,               // 字号
                   HorizontalAlign hAlign,        // 水平对齐方式
                   VerticalAlign vAlign,          // 垂直对齐方式
                   double maxWidth,               // 最大宽度
                   double lineSpacing,            // 行间距
                   double wordSpacing)            // 字间距
                : Base({name, 0, 0}, transform) {
            cairo_set_source_rgba(cr, color.r(), color.g(), color.b(), color.a());

            PangoLayout * layout;
            PangoFontDescription *desc;

            layout = pango_cairo_create_layout(cr);
            pango_layout_set_text(layout, content.c_str(), -1);                      // 文字内容
            desc = pango_font_description_new();                                     // 字体
            pango_font_description_set_family(desc, fontFamily.c_str());             // 字体
            pango_font_description_set_weight(desc, (PangoWeight) fontWeight);       // 字体
            pango_font_description_set_size(desc, fontSize * PANGO_SCALE * 72 / 96); // 字体
            pango_layout_set_font_description(layout, desc);                         // 字体
            pango_font_description_free(desc);                                       // 字体

            if (maxWidth) {
                pango_layout_set_width(layout, maxWidth * PANGO_SCALE);  // 设置界定框
                pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END); // 超出用省略号
            }

            pango_layout_set_spacing(layout, lineSpacing * PANGO_SCALE); // 行距

            // 水平对齐
            switch (hAlign) {
                default:
                case HorizontalAlign::Left:
                    pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
                    break;
                case HorizontalAlign::Center:
                    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
                    break;
                case HorizontalAlign::Right:
                    pango_layout_set_alignment(layout, PANGO_ALIGN_RIGHT);
                    break;
            }

            pango_cairo_update_layout(cr, layout);
            int layoutWidth, layoutHeight;
            pango_layout_get_size(layout, &layoutWidth, &layoutHeight); // 获取实际大小
            _realW = (double) layoutWidth / PANGO_SCALE;
            _realH = (double) layoutHeight / PANGO_SCALE;

            // 水平对齐
            double xMove = 0;
            switch (hAlign) {
                default:
                case HorizontalAlign::Left:
                    xMove = 0;
                    break;
                case HorizontalAlign::Center:
                    xMove = -_realW / 2;
                    break;
                case HorizontalAlign::Right:
                    xMove = -_realW;
                    break;
            }
            // 垂直对齐
            double yMove = 0;
            switch (vAlign) {
                default:
                case VerticalAlign::BaseLine:
                    yMove = -(double) pango_layout_get_baseline(layout) / PANGO_SCALE;
                    break;
                case VerticalAlign::Top:
                    yMove = 0;
                    break;
                case VerticalAlign::Center:
                    yMove = -_realH / 2;
                    break;
                case VerticalAlign::Bottom:
                    yMove = -_realH;
                    break;
            }
            // 移动
            cairo_move_to(cr, xMove, yMove);

            pango_cairo_show_layout(cr, layout);

            g_object_unref(layout);

            // cairo_show_text (cr, text);
            // cairo_text_path(cr, text.c_str());
            // cairo_fill(cr);
        }

        Text::~Text() {
        }

    } // namespace Component
} // namespace ArtRobot
