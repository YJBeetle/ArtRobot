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

    //char *outfile;
    double page_width;
    double page_height;
    surface_type out_type;

    cairo_surface_t *surface;//介质
    cairo_t *cr;//画笔

    int8_t init(const char *filename,surface_type type,double width,double height);
    int8_t uninit();

    JsonParser *jsondata_parser;
    JsonNode *jsondata_node;
    JsonReader *jsondata_reader;
    int8_t jsondata_init(const char *jsondata);
    int32_t jsondata_count();
    int8_t jsondata_read_member(const char *member);
    int8_t jsondata_end_member();
    int8_t jsondata_read_element(int32_t i);
    int8_t jsondata_end_element();
    const char * jsondata_getitem(const char *item);

    int8_t draw_svg(const char *svgfilename, double x, double y, double width, double height);
    int8_t draw_png(const char *pngfilename, double x, double y, double width, double height);
    int8_t draw_text(const char *text, const char *family, double font_size, char alignment, color argb, double x, double y);
    int8_t draw_rectangle(color argb, double x, double y, double width, double height);

    int8_t filecheck(const char *filename);

public:
    draw();
    int8_t make();

};

#endif // DRAW_H
