/*
 * Copyright 2020 YJBeetle
 *
 * Authors:
 *  YJBeetle <YJBeetle@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 3.  See
 * the LICENSE file in the top-level directory.
 *
 */

#include "./Image.hpp"

#include <iostream>

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

Image::Image(std::string __name,
             double x, double y,
             double w, double h,
             double r,
             unsigned char * imageData,
             int imageW, int imageH,
             int imageStride,
             char imagePixelBits,
             bool premultipliedAlpha)
    : Base(TypeImage, __name, x, y, w, h, r)
{
    cairo_surface_t *imageSurface;

    cairo_format_t format;
    switch (imagePixelBits)
    {
    case 1:
        format = CAIRO_FORMAT_A1;
        break;
    case 8:
        format = CAIRO_FORMAT_A8;
        break;
    case 24:
        format = CAIRO_FORMAT_RGB24;
        break;
    case 32:
        format = CAIRO_FORMAT_ARGB32;
        break;
    default:
        format = CAIRO_FORMAT_A1;
        break;
    }

    // 计算预乘
    if (imagePixelBits == 32 && premultipliedAlpha == false)
    {
        // 尝试 cairo_set_operator CAIRO_OPERATOR_OVER CAIRO_OPERATOR_SOURCE ?
        for (int y = 0; y < imageH; y++)
            for (int x = 0; x < imageW; x++)
            {
                auto p = imageData + y * imageStride + x * 4;
                p[0] = (unsigned short)p[0] * p[3] / 0xff;
                p[1] = (unsigned short)p[1] * p[3] / 0xff;
                p[2] = (unsigned short)p[2] * p[3] / 0xff;
            }
    }

    imageSurface = cairo_image_surface_create_for_data(imageData,
                                                       format,
                                                       imageW,
                                                       imageH,
                                                       imageStride);

    drawImageSurface(cr,
                     _w, _h,
                     imageSurface);

    cairo_surface_destroy(imageSurface); // 回收
}

std::shared_ptr<Image> Image::fromMat(std::string __name,
                                      double __x, double __y,
                                      double __w, double __h,
                                      double __r,
                                      const cv::Mat &imageMatRead)
{
    cv::Mat imageMat;
    if (imageMat.channels() == 3)
    {
        cvtColor(imageMatRead, imageMat, cv::COLOR_BGR2BGRA);
    }
    else
    {
        imageMat = std::move(imageMatRead);
        for (int y = 0; y < imageMat.rows; y++) // 预乘
            for (int x = 0; x < imageMat.cols; x++)
            {
                auto &p = imageMat.at<cv::Vec4b>(y, x);
                p[0] = p[0] * p[3] / 0xff;
                p[1] = p[1] * p[3] / 0xff;
                p[2] = p[2] * p[3] / 0xff;
            }
    }

    return std::make_shared<Image>(__name, __x, __y, __w, __h, __r, imageMat.data, imageMat.cols, imageMat.rows, imageMat.step, imageMat.channels() * 8, true);
}

std::shared_ptr<Image> Image::fromFileByCV(std::string __name,
                                           double __x, double __y,
                                           double __w, double __h,
                                           double __r,
                                           const std::string &imageFilePath)
{
    return Image::fromMat(__name, __x, __y, __w, __h, __r, cv::imread(imageFilePath, cv::IMREAD_UNCHANGED));
}

Image::~Image()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
