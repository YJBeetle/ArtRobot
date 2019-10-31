#ifndef DRAW_H
#define DRAW_H
#include "Color.h"

class Draw
{
public:
    enum outputType
    {
        outputTypeUnknow = 0,
        SVG,
        PDF,
        PNG,
        JPG,
    };

    enum unitType
    {
        unitTypeUnknow = 0,
        PX,   // PT
        INCH, // IN
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
    Draw(const string &filename,
         outputType type,
         double width,
         double height,
         unitType unit,
         double ppi);
    ~Draw();

    void nextpage();

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

#endif // DRAW_H
