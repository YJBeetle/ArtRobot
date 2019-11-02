#pragma once

#include "default.h"

#include "Color.h"

#define MM2IN(MM) ((double)MM / 25.4)
#define PT2IN(PT) ((double)PT / 72)

namespace Render
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

    cairo_surface_t *surface; //介质
    cairo_t *cr;              //画笔

public:
    Renderer(double __width,
             double __height,
             unitType __unit,
             double __ppi);
    ~Renderer();

    void render(cairo_surface_t *__surface);

    void saveToFile(string outputPath, OutputType outputType);
};

} // namespace Render
