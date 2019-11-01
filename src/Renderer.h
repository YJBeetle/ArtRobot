#ifndef RENDERER_H
#define RENDERER_H

#include "Color.h"

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

    void save(string outputPath, OutputType outputType);

    int8_t draw_rectangle(Color argb, double x, double y, double width, double height);
    int8_t draw_text(const string &text,
                     const string &fontfile,
                     long face_index,
                     double font_size,
                     int8_t alignment,
                     Color argb,
                     double x, double y);
    int8_t draw_svg(const string &svgfilename,
                    double x, double y,
                    double width, double height);
    int8_t draw_png(const string &pngfilename,
                    double x, double y,
                    double width, double height);
};

cairo_status_t writeCairo(void *closure, const unsigned char *data, unsigned int length);

#define MM2IN(MM) ((double)MM / 25.4)
#define PT2IN(PT) ((double)PT / 72)

#endif // RENDERER_H
