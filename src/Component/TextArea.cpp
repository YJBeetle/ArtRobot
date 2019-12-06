#include "ArtRobot/Component/TextArea.h"

namespace ArtRobot
{
namespace Component
{

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
                   int8_t verticalAlign)          // 垂直对齐方式，0为第一行基线对齐，1为顶部对齐，2垂直居中对齐，3底部对齐
    : Base(TypeText, __name, __x, __y, __w, __h, __r)
{
    cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());

    PangoLayout *layout;
    PangoFontDescription *desc;

    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, content.c_str(), -1); // 文字
    desc = pango_font_description_new();                // 字体
    pango_font_description_set_family(desc, fontFamily.c_str());
    pango_font_description_set_weight(desc, (PangoWeight)fontWeight);
    pango_font_description_set_size(desc, fontSize * PANGO_SCALE * 72 / 96);
    pango_layout_set_font_description(layout, desc); // 字体
    pango_font_description_free(desc);

    pango_layout_set_width(layout, w * PANGO_SCALE);
    pango_layout_set_height(layout, h * PANGO_SCALE);

    pango_cairo_update_layout(cr, layout);

    // 对齐
    int width, height;
    pango_layout_get_size(layout, &width, &height);
    // 水平对齐
    int xMove = 0;
    switch (horizontalAlign)
    {
    default:
    case 0: // 左对齐
        xMove = 0;
        break;
    case 1: // 居中
        xMove = -((double)width / PANGO_SCALE) / 2;
        break;
    case 2: // 右对齐
        xMove = -((double)width / PANGO_SCALE);
        break;
    }
    // 垂直对齐
    int yMove = 0;
    switch (verticalAlign)
    {
    default:
    case 0: // 上对齐
        yMove = 0;
        break;
    case 1: // 中对齐
        yMove = -((double)height / PANGO_SCALE) / 2;
        break;
    case 2: // 下对齐
        yMove = -((double)height / PANGO_SCALE);
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

TextArea::~TextArea()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
