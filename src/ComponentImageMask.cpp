#include "default.h"

#include "ComponentImage.h"
#include "ComponentImageMask.h"

namespace Render
{

void drawMat(cairo_t *cr,
             double w, double h,
             const Mat &maskImageMatRead,
             cairo_surface_t *childSurface)
{
    Mat maskImageMat;
    if (maskImageMatRead.channels() == 3)
        cvtColor(maskImageMatRead, maskImageMat, COLOR_BGR2BGRA);
    else
        maskImageMat = maskImageMatRead;

    for (int y = 0; y < maskImageMat.rows; y++)
        for (int x = 0; x < maskImageMat.cols; x++)
        {
            auto &p = maskImageMat.at<Vec4b>(y, x);
            if (p[3] == 0xFF)
                p[0] = p[1] = p[2] = p[3] = (p[2] * 30 + p[1] * 59 + p[0] * 11) / 100;
            else
                p[0] = p[1] = p[2] = p[3];
        }
    // imwrite("Debug.png", maskImageMat); // Debug

    cairo_surface_t *maskImageSurface = cairo_image_surface_create_for_data(maskImageMat.data,
                                                                            CAIRO_FORMAT_ARGB32,
                                                                            maskImageMat.cols,
                                                                            maskImageMat.rows,
                                                                            maskImageMat.step);

    cairo_surface_t *maskSurface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
    cairo_t *maskCr = cairo_create(maskSurface);
    drawImageSurface(maskCr,
                     w, h,
                     maskImageSurface);

    cairo_set_source_surface(cr, childSurface, 0.0, 0.0);
    cairo_mask_surface(cr, maskSurface, 0, 0);
    cairo_fill(cr);

    cairo_surface_destroy(maskImageSurface); // 回收
    cairo_destroy(maskCr);
    cairo_surface_destroy(maskSurface);
}

ComponentImageMask::ComponentImageMask(double x, double y,
                                       double w, double h,
                                       double r,
                                       const string &maskImageFilePath,
                                       cairo_surface_t *childSurface)
    : Component(x, y, w, h, r)
{
    type = ComponentTypeImageMask;

    auto maskImageMatRead = imread(maskImageFilePath, IMREAD_UNCHANGED);
    drawMat(cr,
            w, h,
            maskImageMatRead,
            childSurface);
}

ComponentImageMask::ComponentImageMask(double x, double y,
                                       double w, double h,
                                       double r,
                                       const Mat &maskImageMatRead,
                                       cairo_surface_t *childSurface)
    : Component(x, y, w, h, r)
{
    type = ComponentTypeImageMask;

    drawMat(cr,
            w, h,
            maskImageMatRead,
            childSurface);
}

ComponentImageMask::~ComponentImageMask()
{
}

} // namespace Render
