#include "default.h"

#include "ComponentSvg.h"

namespace Render
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

ComponentSvg::ComponentSvg(double x, double y,
                           double w, double h,
                           double r,
                           const string &src)
    : Component(x, y, w, h, r)
{
    type = ComponentTypeSvg;

    FILE *imageFile = fopen(src.c_str(), "rb"); // 判断文件存在
    if (imageFile)
    {
        fclose(imageFile);

        RsvgHandle *svg = rsvg_handle_new_from_file(src.c_str(), NULL); // TODO 错误处理

        drawSvg(cr,
                w, h,
                svg);

        rsvg_handle_close(svg, NULL); // 释放handle
    }
}

ComponentSvg::ComponentSvg(double x, double y,
                           double w, double h,
                           double r,
                           unsigned char *data, size_t len)
    : Component(x, y, w, h, r)
{
    type = ComponentTypeSvg;

    RsvgHandle *svg = rsvg_handle_new_from_data(data, len, NULL); // TODO 错误处理

    drawSvg(cr,
            w, h,
            svg);

    rsvg_handle_close(svg, NULL);
}

ComponentSvg::~ComponentSvg()
{
}

} // namespace Render
