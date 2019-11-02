#include "default.h"

// #include <magic.h>

#include "ComponentImage.h"

namespace Render
{

void drawImageSurface(cairo_t *cr, cairo_surface_t *imageSurface,
                      int imageW, int imageH,
                      double x, double y,
                      double w, double h)
{
    cairo_translate(cr, x, y);
    if (w || h)
    {
        double scaleX, scaleY;
        scaleX = w / (double)imageW;
        scaleY = h / (double)imageH;
        cairo_scale(cr, scaleX, scaleY);
    }
    cairo_set_source_surface(cr, imageSurface, 0, 0);
    cairo_paint(cr);
}

void drawMat(cairo_t *cr, const Mat &imageMatRead,
             double x, double y,
             double w, double h)
{
    cairo_surface_t *imageSurface;
    if (imageMatRead.channels() == 3)
    {
        Mat imageMat;
        cvtColor(imageMatRead, imageMat, COLOR_BGR2BGRA);
        imageSurface = cairo_image_surface_create_for_data(imageMat.data,
                                                           CAIRO_FORMAT_RGB24,
                                                           imageMat.cols,
                                                           imageMat.rows,
                                                           imageMat.step);
    }
    else
        imageSurface = cairo_image_surface_create_for_data(imageMatRead.data,
                                                           CAIRO_FORMAT_ARGB32,
                                                           imageMatRead.cols,
                                                           imageMatRead.rows,
                                                           imageMatRead.step);
    // cairo_surface_finish(imageSurface);

    drawImageSurface(cr, imageSurface,
                     imageMatRead.cols, imageMatRead.rows,
                     x, y,
                     w, h);

    cairo_surface_destroy(imageSurface); // 回收
}

ComponentImage::ComponentImage(const string &imageFilePath,
                               double x, double y,
                               double w, double h)
{
    type = ComponentTypeImage;

    cairo_t *cr = cairo_create(surface);

    FILE *imageFile = fopen(imageFilePath.c_str(), "rb"); // 判断文件存在
    if (imageFile)
    {
        fclose(imageFile);
        // TODO: 判断文件类型可以考虑用magic
        // magic_t cookie;
        // cookie = magic_open(MAGIC_MIME_TYPE);
        // magic_load(cookie, NULL);
        // cout << (char *)magic_buffer(cookie, fileDataBuffer, fileDataBufferLength) << endl;
        ImageFileType imageFileType;
        const char *ext = imageFilePath.c_str() + imageFilePath.length() - 4; // 目前用扩展名判断
        if (!strcasecmp(ext, ".svg"))
            imageFileType = SVG;
        else if (!strcasecmp(ext, ".png"))
            imageFileType = PNG;
        else if (!strcasecmp(ext, ".jpg"))
            imageFileType = JPG;

        switch (imageFileType)
        {
        case SVG:
        {
            RsvgHandle *svg;
            svg = rsvg_handle_new_from_file(imageFilePath.c_str(), NULL); // TODO 错误处理   // or rsvg_handle_new_from_data

            cairo_translate(cr, x, y);
            if (w || h)
            {
                unsigned int svg_width, svg_height;
                double scaleX, scaleY;
                RsvgDimensionData dimension_data;
                rsvg_handle_get_dimensions(svg, &dimension_data);
                svg_width = dimension_data.width;
                svg_height = dimension_data.height;
                scaleX = w / (double)svg_width;
                scaleY = h / (double)svg_height;
                cairo_scale(cr, scaleX, scaleY);
            }
            rsvg_handle_render_cairo(svg, cr);

            rsvg_handle_close(svg, NULL); //释放handle
        }
        break;
        case PNG:
        {
            cairo_surface_t *img = cairo_image_surface_create_from_png(imageFilePath.c_str());

            drawImageSurface(cr, img,
                             cairo_image_surface_get_width(img), cairo_image_surface_get_height(img),
                             x, y,
                             w, h);

            cairo_surface_destroy(img); // 回收PNG
        }
        break;
        case JPG:
        default:
        {
            auto imageMatRead = imread(imageFilePath, IMREAD_UNCHANGED);
            drawMat(cr, imageMatRead, x, y, w, h);
        }
        break;
        }
    }

    cairo_destroy(cr);
}

ComponentImage::ComponentImage(const Mat &imageMatRead,
                               double x, double y,
                               double w, double h)
{
    type = ComponentTypeImage;

    cairo_t *cr = cairo_create(surface);

    drawMat(cr, imageMatRead, x, y, w, h);

    cairo_destroy(cr);
}

ComponentImage::~ComponentImage()
{
}

} // namespace Render
