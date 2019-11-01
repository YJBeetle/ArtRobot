#include "default.h"

#include "ComponentImage.h"

namespace Render
{

ComponentImage::ComponentImage(const string &imageFilePath,
                               double x, double y,
                               double width, double height)
{
    type = ComponentTypeImage;

    cairo_t *cr = cairo_create(surface);

    FILE *imageFile = fopen(imageFilePath.c_str(), "rb");
    if (imageFile)
    {
        ImageFileType imageFileType = SVG;
        // TODO: 判断文件类型
        fclose(imageFile); // TODO

        switch (imageFileType)
        {
        case SVG:
        {
            RsvgHandle *svg;
            svg = rsvg_handle_new_from_file(imageFilePath.c_str(), NULL); // TODO 错误处理   // or rsvg_handle_new_from_data

            cairo_translate(cr, x, y);
            if (width || height)
            {
                unsigned int svg_width, svg_height;
                double scaleX, scaleY;
                RsvgDimensionData dimension_data;
                rsvg_handle_get_dimensions(svg, &dimension_data);
                svg_width = dimension_data.width;
                svg_height = dimension_data.height;
                scaleX = width / (double)svg_width;
                scaleY = height / (double)svg_height;
                cairo_scale(cr, scaleX, scaleY);
            }
            rsvg_handle_render_cairo(svg, cr);

            rsvg_handle_close(svg, NULL); //释放handle
        }
        break;
        case PNG:
        {
            cairo_surface_t *img = NULL;
            img = cairo_image_surface_create_from_png(imageFilePath.c_str());

            cairo_translate(cr, x, y);
            if (width || height)
            {
                unsigned int png_width, png_height;
                double scaleX, scaleY;
                png_width = cairo_image_surface_get_width(img);
                png_height = cairo_image_surface_get_height(img);
                scaleX = width / (double)png_width;
                scaleY = height / (double)png_height;
                cairo_scale(cr, scaleX, scaleY);
            }
            cairo_set_source_surface(cr, img, 0, 0);
            cairo_paint(cr);

            cairo_surface_destroy(img); //回收PNG介质
        }
        break;
        case JPG:
        {
            // TODO cairo_image_surface_create_for_data
        }
        break;
        default:
            break;
        }
    }

    cairo_destroy(cr);
}

ComponentImage::~ComponentImage()
{
}

} // namespace Render
