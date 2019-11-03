#include "default.h"

#include "ComponentImage.h"
#include "ComponentImageMask.h"

namespace Render
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
            if (p[3] == 0xFF) // 优先使用Alpha
                p[0] = p[1] = p[2] = p[3] = (p[2] * 30 + p[1] * 59 + p[0] * 11) / 100;
            else
                p[0] = p[1] = p[2] = p[3];
        }
    // imwrite("Debug.png", maskImageMat); // Debug

    ComponentImage maskImage(x, y, w, h, r, maskImageMat);

    cairo_set_source_surface(cr, childSurface, 0.0, 0.0);
    cairo_mask_surface(cr, maskImage.getSurface(), 0, 0);
    cairo_fill(cr);
}

ComponentImageMask::ComponentImageMask(double x, double y,
                                       double w, double h,
                                       double r,
                                       const string &maskImageFilePath,
                                       cairo_surface_t *childSurface)
    : Component()
{
    type = ComponentTypeImageMask;

    auto maskImageMatRead = imread(maskImageFilePath, IMREAD_UNCHANGED);
    drawChildSurfaceUsedMask(cr,
                             x, y,
                             w, h,
                             r,
                             maskImageMatRead,
                             childSurface);
}

ComponentImageMask::ComponentImageMask(double x, double y,
                                       double w, double h,
                                       double r,
                                       const Mat &maskImageMatRead,
                                       cairo_surface_t *childSurface)
    : Component()
{
    type = ComponentTypeImageMask;

    drawChildSurfaceUsedMask(cr,
                             x, y,
                             w, h,
                             r,
                             maskImageMatRead,
                             childSurface);
}

ComponentImageMask::~ComponentImageMask()
{
}

} // namespace Render
