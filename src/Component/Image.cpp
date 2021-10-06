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
#include <jpeglib.h>

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
             double r)
    : Base(TypeImage, __name, x, y, w, h, r)
{
}

Image::Image(std::string __name,
             double x, double y,
             double w, double h,
             double r,
             cairo_surface_t * imageSurface)
    : Base(TypeImage, __name, x, y, w, h, r)
{
    drawImageSurface(cr,
                     _w, _h,
                     imageSurface);
}

std::shared_ptr<Image> Image::fromRaw(std::string __name,
                                      double x, double y,
                                      double w, double h,
                                      double r,
                                      unsigned char * imageData,
                                      int imageW, int imageH,
                                      int imageStride,
                                      char imagePixelBits,
                                      bool premultipliedAlpha)
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

    std::shared_ptr<Image> ret = std::make_shared<Image>(__name, x, y, w, h, r,
                                                         imageSurface);

    cairo_surface_destroy(imageSurface); // 回收

    return ret;
}

std::shared_ptr<Image> Image::fromMat(std::string __name,
                                      double x, double y,
                                      double w, double h,
                                      double r,
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

    return Image::fromRaw(__name, x, y, w, h, r,
                          imageMat.data, imageMat.cols, imageMat.rows, imageMat.step, imageMat.channels() * 8, true);
}

std::shared_ptr<Image> Image::fromFileByCV(std::string __name,
                                           double x, double y,
                                           double w, double h,
                                           double r,
                                           const std::string &imageFilePath)
{
    return Image::fromMat(__name, x, y, w, h, r,
                          cv::imread(imageFilePath, cv::IMREAD_UNCHANGED));
}

std::shared_ptr<Image> Image::fromPNG(std::string __name,
                                      double x, double y,
                                      double w, double h,
                                      double r,
                                      const std::string &imageFilePath)
{
    std::shared_ptr<Image> ret;

    FILE *imageFile = fopen(imageFilePath.c_str(), "rb");

    if (imageFile)
    {
        fclose(imageFile);

        cairo_surface_t *img = cairo_image_surface_create_from_png(imageFilePath.c_str());

        ret = std::make_shared<Image>(__name, x, y, w, h, r, img);

        cairo_surface_destroy(img); // 回收PNG
    }

    return ret;
}

std::shared_ptr<Image> Image::fromJPG(std::string __name,
                                      double x, double y,
                                      double w, double h,
                                      double r,
                                      const std::string &imageFilePath)
{
    std::shared_ptr<Image> ret;

    const char *filename = imageFilePath.c_str();

    FILE *infile;      /* source file */
    JSAMPARRAY buffer; /* Output row buffer */
    int row_stride;    /* physical row width in output buffer */

    //文件检查
    if ((infile = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "文件不存在： %s \n", filename);
        return std::make_shared<Image>(__name, x, y, w, h, r);
    }

    struct jpeg_decompress_struct cinfo;

    //出错处理
    struct jpeg_error_mgr error_mgr;
    cinfo.err = jpeg_std_error(&error_mgr);
    error_mgr.error_exit = [](j_common_ptr cinfo){
        (*cinfo->err->output_message)(cinfo);
    };

    //准备
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void)jpeg_read_header(&cinfo, TRUE);
    (void)jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    // 申请内存
    uint8_t *image_buffer = (uint8_t *)malloc(cinfo.output_width * cinfo.output_height * 4);

    //开始读取
    uint8_t *image_buffer_p = image_buffer;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(image_buffer_p, buffer[0], row_stride);
        for (int i = 0; i < cinfo.output_width; i++)
        {
            image_buffer_p[i * 4 + 1] = buffer[0][i * cinfo.output_components + 0];
            image_buffer_p[i * 4 + 2] = buffer[0][i * cinfo.output_components + 1];
            image_buffer_p[i * 4 + 3] = buffer[0][i * cinfo.output_components + 2];
        }
        image_buffer_p += cinfo.output_width * 4;
        // put_scanline_someplace(buffer[0], row_stride);
    }

    ret = Image::fromRaw(__name, x, y, w, h, r,
                         image_buffer, cinfo.output_width, cinfo.output_height, cinfo.output_width * 4, 4 * 8, true);

    free(image_buffer);

    //清理
    (void)jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    return ret;
}

std::shared_ptr<Image> Image::fromFile(std::string __name,
                                       double x, double y,
                                       double w, double h,
                                       double r,
                                       const std::string &imageFilePath)
{
    const char *ext = imageFilePath.c_str() + imageFilePath.length() - 4;
    if (!strcasecmp(ext, ".png"))
        return Image::fromPNG(__name, x, y, w, h, r, imageFilePath);
    else if (!strcasecmp(ext, ".jpg"))
        return Image::fromJPG(__name, x, y, w, h, r, imageFilePath);
    return std::make_shared<Image>(__name, x, y, w, h, r);
}

Image::~Image()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
