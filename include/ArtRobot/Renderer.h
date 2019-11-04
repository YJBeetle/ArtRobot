#pragma once

#include <string>
#include <vector>
#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>

using namespace std;

#include "ArtRobot/Types/OutputType.h"

#define MM2IN(MM) ((double)MM / 25.4)
#define PX2IN(PX, PPI) ((double)PX / PPI)

namespace ArtRobot
{

class Renderer
{
public:
    enum unitType
    {
        unitTypeUnknow = 0,
        PX, // PT
        IN, // INCH
        MM,
        CM,
    };

private:
    double surfaceWidth;
    double surfaceHeight;
    double ppi;
    OutputType outputType;

    cairo_surface_t *surface; //介质
    cairo_t *cr;              //画笔

    vector<unsigned char> data;

public:
    Renderer(OutputType __outputType,
             double __width,
             double __height,
             unitType __unit,
             double __ppi);
    ~Renderer();

    void render(cairo_surface_t *__surface);

    void saveToFile(string outputPath);
};

} // namespace ArtRobot
