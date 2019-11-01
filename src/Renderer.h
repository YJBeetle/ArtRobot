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
    FILE *out_file;
    outputType surface_type;
    double surface_width;
    double surface_height;

    cairo_surface_t *surface; //介质
    cairo_t *cr;              //画笔

    int8_t filecheck(const char *filename);

public:
    Renderer(const string &filename,
         outputType type,
         double width,
         double height,
         unitType unit,
         double ppi);
    ~Renderer();

    // void nextpage(); // 换页函数，但是现在重写后多页格式还没想好

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
