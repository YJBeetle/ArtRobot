#include "ArtRobot/Component/Text.h"

namespace ArtRobot
{
namespace Component
{

Text::Text(std::string __name,
           double __x, double __y,
           double __w, double __h,
           double __r,
           const std::string &content,
           const std::string &fontFamily,
           int fontWeight,
           double fontSize,
           int8_t alignment,
           Color color) // TODO 此处渲染文字仅为测试 正式的排版考虑使用Pango
    : Base(TypeText, __name, __x, __y, __w, __h, __r)
{
    cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());

    PangoLayout *layout;
    PangoFontDescription *desc;

    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, content.c_str(), -1);
    desc = pango_font_description_new();
    pango_font_description_set_family(desc, fontFamily.c_str());
    pango_font_description_set_weight(desc, (PangoWeight)fontWeight);
    pango_font_description_set_size(desc, fontSize * PANGO_SCALE * 72 / 96);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_cairo_update_layout(cr, layout);

    // 基线
    int baseline = pango_layout_get_baseline(layout);
    cairo_move_to(cr, 0, -(double)baseline / PANGO_SCALE);

    // 居中
    // int width, height;
    // pango_layout_get_size(layout, &width, &height);
    // cairo_move_to(cr, -((double)width / PANGO_SCALE) / 2, 0);

    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    // cairo_show_text (cr, text);
    // cairo_text_path(cr, text.c_str());
    // cairo_fill(cr);
}

Text::~Text()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
