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

#ifdef WIN32
#include <fcntl.h>
#endif
#include <webp/encode.h>

#include "ArtRobot/Renderer.h"

namespace ArtRobot
{

cairo_status_t writeStreamToData(void *closure, const unsigned char *data, unsigned int length)
{
    auto *vecData = (std::vector<unsigned char> *)closure;
    vecData->insert(vecData->end(), data, data + length);
    return CAIRO_STATUS_SUCCESS;
}

Renderer::Renderer(OutputType __outputType,
                   double __width,
                   double __height,
                   unitType __unit,
                   double __ppi)
    : outputType(__outputType),
      ppi(__ppi)
{
    double scale;
    switch (__unit)
    {
    default:
    case unitTypeUnknow:
    case PX:
        this->surfaceWidth = PX2IN(__width, ppi);
        this->surfaceHeight = PX2IN(__height, ppi);
        scale = PX2IN(1, ppi);
        break;
    case IN:
        this->surfaceWidth = __width;
        this->surfaceHeight = __height;
        scale = 1;
        break;
    case MM:
        this->surfaceWidth = MM2IN(__width);
        this->surfaceHeight = MM2IN(__height);
        scale = MM2IN(1);
        break;
    case CM:
        this->surfaceWidth = MM2IN(__width) * 10;
        this->surfaceHeight = MM2IN(__height) * 10;
        scale = MM2IN(1) * 10;
        break;
    }

    switch (outputType)
    {
    default:
    case OutputTypeUnknow:
    case OutputTypeSvg:
        surface = cairo_svg_surface_create_for_stream(writeStreamToData,
                                                      (void *)&data,
                                                      surfaceWidth * ppi,
                                                      surfaceHeight * ppi); //默认单位pt
        break;
    case OutputTypePdf:
        surface = cairo_pdf_surface_create_for_stream(writeStreamToData,
                                                      (void *)&data,
                                                      surfaceWidth * ppi,
                                                      surfaceHeight * ppi); //默认单位是mm，所以需要mm转inch
        cairo_surface_set_fallback_resolution(surface, ppi, ppi);           //设置分辨率
        // cairo_show_page(cr);                                             // 多页
        break;
    case OutputTypePng:
    case OutputTypeWebp:
    case OutputTypePixmap:
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                             surfaceWidth * ppi,
                                             surfaceHeight * ppi); //默认单位pt
        break;
    }

    cr = cairo_create(surface);                //创建画笔
    cairo_scale(cr, scale * ppi, scale * ppi); //缩放画笔
}

Renderer::~Renderer()
{
    cairo_destroy(cr);              //回收画笔
    cairo_surface_destroy(surface); //回收介质
}

void Renderer::render(cairo_surface_t *__surface)
{
    cairo_set_source_surface(cr, __surface, 0.0, 0.0);
    cairo_paint(cr);

    switch (outputType)
    {
    case OutputTypeSvg:
    case OutputTypePdf:
        cairo_surface_finish(surface);
        break;
    case OutputTypePng: // PNG需要在渲染完成之后再写入data
        data.clear();   // 防止重复渲染
        cairo_surface_write_to_png_stream(surface, writeStreamToData, (void *)&data);
        break;
    case OutputTypeWebp:
        data.clear();
        {
            uint8_t *output = nullptr;
            auto pixData = cairo_image_surface_get_data(surface);
            auto pixWidth = cairo_image_surface_get_width(surface);
            auto pixHeight = cairo_image_surface_get_height(surface);
            auto pixStride = cairo_image_surface_get_stride(surface);
            auto pixLength = pixHeight * pixStride;
            // auto pixDataNew = new uint8_t[pixLength];
            // memcpy(pixDataNew, pixData, pixLength);
            // auto pixDataP = pixDataNew;
            auto pixDataP = pixData;
            for (size_t y = 0; y < pixHeight; y++) // 反预乘
            {
                for (size_t x = 0; x < pixWidth; x++)
                {
                    auto p = pixDataP + x * 4;
                    if (p[3])
                    {
                        p[0] = p[0] * 0xff / p[3];
                        p[1] = p[1] * 0xff / p[3];
                        p[2] = p[2] * 0xff / p[3];
                    }
                    else
                    {
                        p[0] = p[1] = p[2] = 0xff;
                    }
                }
                pixDataP += pixStride;
            }
            auto outputSize = WebPEncodeBGRA(pixData, pixWidth, pixHeight, pixStride, 100, &output);
            // if (pixDataNew)
            //     delete pixDataNew;
            if (outputSize && output)
            {
                data.insert(data.begin(), output, output + outputSize);
                free(output);
            }
        }
        break;
    case OutputTypePixmap:
        data.clear();
        {
            auto pixData = cairo_image_surface_get_data(surface);
            data.insert(data.begin(), pixData, pixData + cairo_image_surface_get_stride(surface) * cairo_image_surface_get_height(surface));
        }
        break;
    case OutputTypeUnknow:
        break;
    }
}

void Renderer::saveToFile(std::string outputPath)
{
    FILE *outputFile;
    if (!outputPath.empty())
    {
        outputFile = fopen(outputPath.c_str(), "wb");
    }
    else
    {
#ifdef WIN32
        _setmode(_fileno(stdout), O_BINARY);
#endif
        outputFile = stdout;
    }

    fwrite(data.data(), data.size(), 1, outputFile);

    if (outputFile != stdout)
        fclose(outputFile);
}

std::vector<unsigned char> Renderer::getData()
{
    return data;
}

unsigned char *Renderer::getDataC()
{
    return data.data();
}

size_t Renderer::getDataCSize()
{
    return data.size();
}

std::string Renderer::getDataString()
{
    return std::string((const char *)&(*data.begin()),
                       data.size());
}

} // namespace ArtRobot
