#include "ArtRobot/Component/Image.h"

namespace ArtRobot
{
namespace Component
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
    {
        Mat imageMat = imageMatRead;
        for (int y = 0; y < imageMat.rows; y++)
            for (int x = 0; x < imageMat.cols; x++)
            {
                auto &p = imageMat.at<Vec4b>(y, x);
                p[0] = p[0] * p[3] / 0xff;
                p[1] = p[1] * p[3] / 0xff;
                p[2] = p[2] * p[3] / 0xff;
            }
        imageSurface = cairo_image_surface_create_for_data(imageMat.data,
                                                           CAIRO_FORMAT_ARGB32,
                                                           imageMat.cols,
                                                           imageMat.rows,
                                                           imageMat.step);
    }

    drawImageSurface(cr,
                     w, h,
                     imageSurface);

    cairo_surface_finish(imageSurface);

    cairo_surface_destroy(imageSurface); // 回收
}

Image::Image(double x, double y,
             double w, double h,
             double r,
             const string &src)
    : Base(x, y, w, h, r)
{
    type = TypeImage;

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

Image::Image(double x, double y,
             double w, double h,
             double r,
             const Mat &imageMatRead)
    : Base(x, y, w, h, r)
{
    type = TypeImage;

    drawMat(cr,
            w, h,
            imageMatRead);
}

Image::~Image()
{
}

} // namespace Component
} // namespace ArtRobot
