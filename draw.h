#ifndef DRAW_H
#define DRAW_H

enum _surface_type{
    PDF,
    SVG
};

typedef enum _surface_type surface_type;

class draw
{
private:
    int8_t initrd;

    char *outfile;
    double page_width;
    double page_height;
    surface_type out_type;

    cairo_surface_t *surface;//介质
    cairo_t *cr;//画笔

    void init(char *filename,surface_type type,double width,double height);

    void draw_svg(char *svgfilename, double x, double y, double width, double height);
    void draw_png(char *pngfilename, double x, double y, double width, double height);
    void draw_text(char *text, char *family, double font_size, char alignment, int32_t color_code, double x, double y);
    void draw_rectangle(int32_t color_code, double x, double y, double width, double height);


public:
    draw();
    int8_t make();

};

#endif // DRAW_H
