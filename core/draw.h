#ifndef DRAW_H
#define DRAW_H
#include "color.h"
#include "json.h"

class draw
{
private:
    int8_t inited;

    Json json;

    const char *out_file;
    const char *out_type;
    double page_width;
    double page_height;
    int64_t page_count;

    cairo_surface_t *surface;//介质
    cairo_t *cr;//画笔

    int8_t init(const char *filename,const char *type,double width,double height,int64_t count);
    int8_t uninit();
    int8_t nextpage();

    int8_t draw_rectangle(Color argb, double x, double y, double width, double height);
    int8_t draw_text(const char *text, const char *fontfile, long face_index, double font_size, int8_t alignment, Color argb, double x, double y);
    int8_t draw_svg(const char *svgfilename, double x, double y, double width, double height);
    int8_t draw_png(const char *pngfilename, double x, double y, double width, double height);

    int8_t filecheck(const char *filename);

public:
    draw();
    int8_t make(const char *jsondata);

};

#endif // DRAW_H
