#include "ArtRobot/Component/Svg.h"

namespace ArtRobot
{
namespace Component
{

void drawSvg(cairo_t *cr,
             double w, double h,
             RsvgHandle *svg)
{
    if (w || h)
    {
        double scaleX, scaleY;
        RsvgDimensionData dimension_data;
        rsvg_handle_get_dimensions(svg, &dimension_data);
        scaleX = w / (double)dimension_data.width;
        scaleY = h / (double)dimension_data.height;
        cairo_scale(cr, scaleX, scaleY);
    }
    rsvg_handle_render_cairo(svg, cr);
}

Svg::Svg(std::string __name,
         double __x, double __y,
         double __w, double __h,
         double __r,
         const std::string &src)
    : Base(TypeSvg, __name, __x, __y, __w, __h, __r)
{
    FILE *imageFile = fopen(src.c_str(), "rb"); // 判断文件存在
    if (imageFile)
    {
        fclose(imageFile);

        RsvgHandle *svg = rsvg_handle_new_from_file(src.c_str(), NULL); // TODO 错误处理

        drawSvg(cr,
                _w, _h,
                svg);

        rsvg_handle_close(svg, NULL); // 释放handle
    }
}

Svg::Svg(std::string __name,
         double __x, double __y,
         double __w, double __h,
         double __r,
         unsigned char *data, size_t len)
    : Base(TypeSvg, __name, __x, __y, __w, __h, __r)
{
    RsvgHandle *svg = rsvg_handle_new_from_data(data, len, NULL); // TODO 错误处理

    drawSvg(cr,
            _w, _h,
            svg);

    rsvg_handle_close(svg, NULL);
}

Svg::~Svg()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
