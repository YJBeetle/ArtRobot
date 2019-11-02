#include "default.h"

#include "ComponentImage.h"
#include "Jpeg.h"

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
        // TODO: 判断文件类型
        ImageFileType imageFileType;
        const char *ext = imageFilePath.c_str() + imageFilePath.length() - 4;
        if (!strcasecmp(ext, ".svg"))
            imageFileType = SVG;
        else if (!strcasecmp(ext, ".png"))
            imageFileType = PNG;
        else if (!strcasecmp(ext, ".jpg"))
            imageFileType = JPG;

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
            cairo_surface_t *img = cairo_image_surface_create_from_png(imageFilePath.c_str());

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

            cairo_surface_destroy(img); // 回收PNG
        }
        break;
        case JPG:
        {
            auto jpg = read_JPEG_file(imageFilePath.c_str());
            cairo_surface_t *img = cairo_image_surface_create_for_data(jpg->getBuffer(),
                                                                       CAIRO_FORMAT_ARGB32,
                                                                       jpg->getWidth(),
                                                                       jpg->getHeight(),
                                                                       jpg->getWidth() * 4);

            // cout<< jpg->getBuffer()<< endl;

            cairo_translate(cr, x, y);
            if (width || height)
            {
                unsigned int png_width, png_height;
                double scaleX, scaleY;
                png_width = jpg->getWidth();
                png_height = jpg->getHeight();
                scaleX = width / (double)png_width;
                scaleY = height / (double)png_height;
                cairo_scale(cr, scaleX, scaleY);
            }
            cairo_set_source_surface(cr, img, 0, 0);
            cairo_paint(cr);

            cairo_surface_destroy(img); // 回收
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
