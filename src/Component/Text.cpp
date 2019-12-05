#include "ArtRobot/Component/Text.h"

namespace ArtRobot
{
namespace Component
{

Text::Text(std::string __name,
           double __x, double __y,
           double __w, double __h,
           double __r,
           const std::string &text,
           const std::string &fontfile,
           long face_index,
           double font_size,
           int8_t alignment,
           Color argb) // TODO 此处渲染文字仅为测试 正式的排版考虑使用Pango
    : Base(TypeText, __name, __x, __y, __w, __h, __r)
{
    if (!face_index)
        face_index = 0;

    FT_Library ft_library;
    FT_Face ft_face;
    cairo_font_face_t *cr_face;
    if (FT_Init_FreeType(&ft_library))
    {
        fprintf(stderr, "Text::Text: warning: FT_Init_FreeType failed.\n");
        return;
    }
    if (FT_New_Face(ft_library, fontfile.c_str(), face_index, &ft_face))
    {
        fprintf(stderr, "Text::Text: error: FT_New_Face failed, maybe font not found.\n");
        return;
    }
    cr_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);
    cairo_set_font_face(cr, cr_face);
    //cairo_select_font_face (cr, family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, font_size);
    cairo_set_source_rgba(cr, argb.red(), argb.green(), argb.blue(), argb.alpha());

    cairo_text_extents_t extents;
    switch (alignment)
    {
    default:
    case 0:
        break;
    case 1:
        cairo_text_extents(cr, text.c_str(), &extents);
        cairo_move_to(cr, -extents.width / 2, 0);
        break;
    case 2:
        cairo_text_extents(cr, text.c_str(), &extents);
        cairo_move_to(cr, -extents.width, 0);
        break;
    }

    //cairo_show_text (cr, text);
    cairo_text_path(cr, text.c_str());
    cairo_fill(cr);
}

Text::~Text()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
