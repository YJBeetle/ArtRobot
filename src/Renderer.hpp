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

#pragma once

#include <ArtRobot/Features.hpp>

#include <string>
#include <vector>
#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>

#define MM2IN(MM) ((double)MM / 25.4)
#define PX2IN(PX, PPI) ((double)PX / PPI)

namespace ArtRobot {

    enum class Unit {
        Pixel,
        Inch,
        Millimeter,
        Centimeter,
    };

    enum class OutputType {
        Svg,
        Pdf,
        Pixmap,
        Png,
#ifdef WEBP_FOUND
        Webp,
#endif
#ifdef JPEG_FOUND
        Jpeg,
#endif
    };

    class Renderer {
    public:

    private:
        double surfaceWidth;
        double surfaceHeight;
        double ppi;
        OutputType outputType;

        cairo_surface_t *surface; //介质
        cairo_t *cr;              //画笔

        std::vector<unsigned char> data;

    public:
        Renderer(OutputType __outputType,
                 double __width,
                 double __height,
                 Unit __unit = ArtRobot::Unit::Pixel,
                 double __ppi = 72);

        ~Renderer();

        void render(cairo_surface_t *__surface);

        void saveToFile(std::string outputPath);

        std::vector<unsigned char> getData();

        unsigned char *getDataC();

        size_t getDataCSize();

        std::string getDataString();
    };

} // namespace ArtRobot
