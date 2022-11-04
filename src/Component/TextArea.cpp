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

#include "./TextArea.hpp"

namespace ArtRobot {
    namespace Component {

        TextArea::TextArea(std::string __name,
                           double __x, double __y,
                           double __w, double __h,
                           double __r,
                           const std::string &content,    // 内容
                           Color color,                   // 颜色
                           const std::string &fontFamily, // 字体
                           int fontWeight,                // 粗细
                           double fontSize,               // 字号
                           int8_t horizontalAlign,        // 水平对齐方式，0为左对齐，1居中，2右对齐
                           int8_t verticalAlign,          // 垂直对齐方式，0为第一行基线对齐，1为顶部对齐，2垂直居中对齐，3底部对齐
                           double lineSpacing,            // 行间距
                           double wordSpacing)            // 字间距
                : Base(TypeText, __name, __x, __y, __w, __h, __r) {
            cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());

            PangoLayout *layout;
            PangoFontDescription *desc;

            layout = pango_cairo_create_layout(cr);
            pango_layout_set_text(layout, content.c_str(), -1);                      // 文字内容
            desc = pango_font_description_new();                                     // 字体
            pango_font_description_set_family(desc, fontFamily.c_str());             // 字体
            pango_font_description_set_weight(desc, (PangoWeight) fontWeight);        // 字体
            pango_font_description_set_size(desc, fontSize * PANGO_SCALE * 72 / 96); // 字体
            pango_layout_set_font_description(layout, desc);                         // 字体
            pango_font_description_free(desc);                                       // 字体

            pango_layout_set_width(layout, _w * PANGO_SCALE);  // 设置界定框
            pango_layout_set_height(layout, _h * PANGO_SCALE); // 设置界定框

            pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);         // 换行方式
            pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);     // 超出用省略号
            // pango_layout_set_justify(layout, true);                      // 对于换行的行，尽可能撑满整行（两端对齐）
            pango_layout_set_spacing(layout, lineSpacing * PANGO_SCALE); // 行距
            // pango_layout_set_indent(layout, 10 * PANGO_SCALE);           // 测试用的行首锁进

            // 水平对齐
            switch (horizontalAlign) {
                default:
                case 0: // 左对齐
                    pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
                    break;
                case 1: // 居中
                    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
                    break;
                case 2: // 右对齐
                    pango_layout_set_alignment(layout, PANGO_ALIGN_RIGHT);
                    break;
            }

            pango_cairo_update_layout(cr, layout);
            int layoutWidth, layoutHeight;
            pango_layout_get_size(layout, &layoutWidth, &layoutHeight); // 获取实际大小
            _realW = (double) layoutWidth / PANGO_SCALE;
            _realH = (double) layoutHeight / PANGO_SCALE;

            // 垂直对齐
            double yMove = 0;
            switch (verticalAlign) {
                default:
                case 0: // 上对齐
                    yMove = 0;
                    break;
                case 1: // 中对齐
                    yMove = (_h - _realH) / 2;
                    break;
                case 2: // 下对齐
                    yMove = _h - _realH;
                    break;
            }
            // 移动
            cairo_move_to(cr, 0, yMove);

            pango_cairo_show_layout(cr, layout);

            g_object_unref(layout);

            // cairo_show_text (cr, text);
            // cairo_text_path(cr, text.c_str());
            // cairo_fill(cr);
        }

        TextArea::~TextArea() {
            finish();
        }

    } // namespace Component
} // namespace ArtRobot
