#include "ArtRobot/Component/ImageMask.h"

namespace ArtRobot
{
namespace Component
{

void drawChildSurfaceUsedMask(cairo_t *cr,
                              double x, double y,
                              double w, double h,
                              double r,
                              const Mat &maskImageMatRead,
                              cairo_surface_t *childSurface)
{
    Mat maskImageMat;
    if (maskImageMatRead.channels() == 3)
        cvtColor(maskImageMatRead, maskImageMat, COLOR_BGR2BGRA);
    else
        maskImageMat = maskImageMatRead;

    // 解决Ai的蒙版兼容性问题，Ai的蒙版使用的是黑白，而Acrobat中使用的是Alpha。
    for (int y = 0; y < maskImageMat.rows; y++)
        for (int x = 0; x < maskImageMat.cols; x++)
        {
            auto &p = maskImageMat.at<Vec4b>(y, x);
            uchar a = p[3];
            uchar b = (p[2] * 30 + p[1] * 59 + p[0] * 11) / 100;
            p[0] = p[1] = p[2] = p[3] = MIN(a, b);
        }

    Image maskImage(x, y, w, h, r, maskImageMat);

    cairo_set_source_surface(cr, childSurface, 0.0, 0.0);
    cairo_mask_surface(cr, maskImage.getSurface(), 0, 0);
    cairo_fill(cr);
}

ImageMask::ImageMask(double x, double y,
                     double w, double h,
                     double r,
                     const string &maskImageFilePath,
                     cairo_surface_t *childSurface)
{
    type = TypeImageMask;

    auto maskImageMatRead = imread(maskImageFilePath, IMREAD_UNCHANGED);
    drawChildSurfaceUsedMask(cr,
                             x, y,
                             w, h,
                             r,
                             maskImageMatRead,
                             childSurface);
}

ImageMask::ImageMask(double x, double y,
                     double w, double h,
                     double r,
                     const Mat &maskImageMatRead,
                     cairo_surface_t *childSurface)
{
    type = TypeImageMask;

    drawChildSurfaceUsedMask(cr,
                             x, y,
                             w, h,
                             r,
                             maskImageMatRead,
                             childSurface);
}

ImageMask::~ImageMask()
{
}

} // namespace Component
} // namespace ArtRobot
