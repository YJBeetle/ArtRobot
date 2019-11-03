#include "default.h"

#include "ComponentImage.h"

namespace Render
{

void drawImageSurface(cairo_t *cr,
                      double w, double h,
                      cairo_surface_t *imageSurface)
{
    if (w || h)
    {
        double scaleX, scaleY;
        scaleX = w / (double)cairo_image_surface_get_width(imageSurface);
        scaleY = h / (double)cairo_image_surface_get_height(imageSurface);
        cairo_scale(cr, scaleX, scaleY);
    }
    cairo_set_source_surface(cr, imageSurface, 0, 0);
    cairo_paint(cr);
}

void drawMat(cairo_t *cr,
             double w, double h,
             const Mat &imageMatRead)
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

    drawImageSurface(cr,
                     w, h,
                     imageSurface);

    cairo_surface_finish(imageSurface);

    cairo_surface_destroy(imageSurface); // 回收
}

ComponentImage::ComponentImage(double x, double y,
                               double w, double h,
                               double r,
                               const string &src)
    : Component(x, y, w, h, r)
{
    type = ComponentTypeImage;

    FILE *imageFile = fopen(src.c_str(), "rb"); // 判断文件存在
    if (imageFile)
    {
        fclose(imageFile);
        auto imageMatRead = imread(src, IMREAD_UNCHANGED);
        drawMat(cr,
                w, h,
                imageMatRead);
    }
}

ComponentImage::ComponentImage(double x, double y,
                               double w, double h,
                               double r,
                               const Mat &imageMatRead)
    : Component(x, y, w, h, r)
{
    type = ComponentTypeImage;

    drawMat(cr,
            w, h,
            imageMatRead);
}

ComponentImage::~ComponentImage()
{
}

} // namespace Render
