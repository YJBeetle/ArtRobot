#include "ArtRobot/Component/ImageMask.h"

namespace ArtRobot
{
namespace Component
{

void ImageMask::drawChildSurfaceUsedMask(const cv::Mat &maskImageMatRead)
{
    if (maskImageMatRead.channels() == 3)
        cvtColor(maskImageMatRead, maskImageMat, cv::COLOR_BGR2BGRA);
    else
        maskImageMat = maskImageMatRead;

    // 解决Ai的蒙版兼容性问题，Ai的蒙版使用的是黑白，而Acrobat中使用的是Alpha。
    for (int y = 0; y < maskImageMat.rows; y++)
        for (int x = 0; x < maskImageMat.cols; x++)
        {
            auto &p = maskImageMat.at<cv::Vec4b>(y, x);
            uchar a = p[3];
            uchar b = (p[2] * 30 + p[1] * 59 + p[0] * 11) / 100;
            p[0] = p[1] = p[2] = p[3] = MIN(a, b);
        }

    Image maskImage(name, x, y, w, h, r, maskImageMat);

    cairo_set_source_surface(cr, child->getSurface(), 0.0, 0.0);
    cairo_mask_surface(cr, maskImage.getSurface(), 0, 0);
    cairo_fill(cr);
}

ImageMask::ImageMask(std::string __name, 
                     double __x, double __y,
                     double __w, double __h,
                     double __r,
                     const std::string &maskImageFilePath,
                     std::shared_ptr<Base> __child)
    : Base(TypeImageMask, __name)
{
    child = __child;

    auto maskImageMatRead = cv::imread(maskImageFilePath, cv::IMREAD_UNCHANGED);
    drawChildSurfaceUsedMask(maskImageMatRead);
}

ImageMask::ImageMask(std::string __name, 
                     double __x, double __y,
                     double __w, double __h,
                     double __r,
                     const cv::Mat &maskImageMatRead,
                     std::shared_ptr<Base> __child)
    : Base(TypeImageMask, __name)
{
    child = __child;

    drawChildSurfaceUsedMask(maskImageMatRead);
}

ImageMask::~ImageMask()
{
}

} // namespace Component
} // namespace ArtRobot
