#ifndef DRAW_H
#define DRAW_H

class draw
{
private:
    int8_t initrd;

    //const char *outfile;
    //const char *out_type;
    double page_width;
    double page_height;

    cairo_surface_t *surface;//介质
    cairo_t *cr;//画笔

    int8_t init(const char *filename,const char *type,double width,double height);
    int8_t uninit();

    int8_t jsondata_initrd;
    JsonParser *jsondata_parser;
    JsonNode *jsondata_node;
    JsonReader *jsondata_reader;
    int8_t jsondata_init(const char *jsondata);
    int32_t jsondata_count();
    int8_t jsondata_read_member(const char *member);
    int8_t jsondata_end_member();
    int8_t jsondata_read_element(int32_t i);
    int8_t jsondata_end_element();
    const char * jsondata_get_string(const char *item);
    int64_t jsondata_get_int(const char *item);
    double jsondata_get_double(const char *item);

    int8_t draw_svg(const char *svgfilename, double x, double y, double width, double height);
    int8_t draw_png(const char *pngfilename, double x, double y, double width, double height);
    int8_t draw_text(const char *text, const char *family, double font_size, int8_t alignment, color argb, double x, double y);
    int8_t draw_rectangle(color argb, double x, double y, double width, double height);

    int8_t filecheck(const char *filename);

public:
    draw();
    int8_t make();

};

#endif // DRAW_H
