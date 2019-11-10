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

void Image::drawMat(const cv::Mat &imageMatRead)
{
    cairo_surface_t *imageSurface;
    if (imageMatRead.channels() == 3)
    {
        cvtColor(imageMatRead, imageMat, cv::COLOR_BGR2BGRA);
        imageSurface = cairo_image_surface_create_for_data(imageMat.data,
                                                           CAIRO_FORMAT_RGB24,
                                                           imageMat.cols,
                                                           imageMat.rows,
                                                           imageMat.step);
    }
    else
    {
        imageMat = std::move(imageMatRead);
        for (int y = 0; y < imageMat.rows; y++)
            for (int x = 0; x < imageMat.cols; x++)
            {
                auto &p = imageMat.at<cv::Vec4b>(y, x);
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

    cairo_surface_destroy(imageSurface); // 回收
}

Image::Image(std::string __name,
             double __x, double __y,
             double __w, double __h,
             double __r,
             const std::string &src)
    : Image(__name, __x, __y, __w, __h, __r, cv::imread(src, cv::IMREAD_UNCHANGED))
{
}

Image::Image(std::string __name,
             double __x, double __y,
             double __w, double __h,
             double __r,
             const cv::Mat &imageMatRead)
    : Base(TypeImage, __name, __x, __y, __w, __h, __r)
{
    drawMat(imageMatRead);
}

Image::~Image()
{
}

} // namespace Component
} // namespace ArtRobot
