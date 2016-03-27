#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <librsvg/rsvg.h>

#include <json-glib/json-glib.h>

#include "default.h"
#include "color.h"
#include "draw.h"

draw::draw()
{
    this->initrd=0;
}

int8_t draw::init(const char *filename,surface_type type,double width,double height)
{
    if(this->initrd)return 1;
    this->initrd=1;

    //this->outfile=filename;
    this->out_type=type;
    this->page_width=width;
    this->page_height=height;

    switch (type) {
    case PDF://PDF
        surface = cairo_pdf_surface_create (filename, MM2PT(page_width), MM2PT(page_height));//创建介质
        //cairo_surface_set_fallback_resolution(surface,300,300);//设置分辨率
        cr = cairo_create (surface);//创建画笔
        cairo_scale (cr, MM2PT(1), MM2PT(1));//缩放画笔，因PDF用mm作为最终单位故需缩放画笔
        break;
    case SVG://SVG
        surface = cairo_svg_surface_create (filename, page_width, page_height);
        cr = cairo_create (surface);//创建画笔
        break;
    default:
        fprintf (stderr, "未知的输出类型\n");
        this->initrd=0;
        return 2;
        break;
    }

    return 0;
}

int8_t draw::make()
{
    const char *jsondata="[{\"type\":\"rectangle\",\"color_code\":\"FCF7E8\"},{\"type\":\"svg\",\"filename\":\"bg-veins.svg\"},{\"type\":\"png\",\"filename\":\"1.png\"},{\"type\":\"text\",\"text\":\"YJBeetle\"}]";
    jsondata_init(jsondata);

    int count=jsondata_count(); //元素个数
    const char *type;

    for(int i=0;jsondata_element_in(i),i!=count;jsondata_element_out(),++i) //循环处理该成员中的元素
    {
        type=jsondata_getitem("type");
        printf("%s\n",type); //输出值
        printf("==========\n");

        if(strstr(type,"rectangle"))
        {
            const char *color=jsondata_getitem("color");
        }
    }






    init("out.pdf",PDF,A3_HEIGHT,A3_WIDTH);

    draw_rectangle("0xFCF7E8", 0, 0, page_width, page_height);
    draw_svg("bg-veins.svg", 0, 0, page_width, page_height);
    draw_png("1.png",100,100,100,100);
    draw_svg("logo.svg", (page_width-50)/2, 30, 50, 37.544);
    draw_rectangle( 0x686767, 0, 0, page_width, 16);
    draw_rectangle( 0x686767, 0, page_height-16, page_width, 16);
    draw_text("Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an", "Lantinghei SC Demibold", 5, 1, 0xffffff, page_width/2, 10);
    draw_text("Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an", "Lantinghei SC Demibold", 5, 1, 0xffffff, page_width/2, page_height-6);
    draw_text("YJBeetle", "Yuanti SC", 20, 1, 0x686767, page_width/2, 100);
    draw_text("A测试中文", "Lantinghei SC Extralight", 20, 1, 0x686767, page_width/2, 120);


    cairo_destroy (cr);//回收画笔
    cairo_surface_destroy (surface);//回收介质

    return 0;
}

int8_t draw::jsondata_init(const char *jsondata)
{
    jsondata_parser=json_parser_new();
    json_parser_load_from_data(jsondata_parser,jsondata,-1,NULL);
    jsondata_node=json_parser_get_root(jsondata_parser); //得到root结点
    jsondata_reader=json_reader_new(jsondata_node); //使用JsonReader来做解析
    return 0;
}

const char * draw::jsondata_getitem(const char *item)
{
    json_reader_read_member(jsondata_reader,item); //得到该元素中的成员
    const char *value=json_reader_get_string_value(jsondata_reader);
    json_reader_end_member(jsondata_reader); //返回上一个节点
    return value;
}

int32_t draw::jsondata_count()
{
    return json_reader_count_elements(jsondata_reader);
}

int8_t draw::jsondata_element_in(int32_t i)
{
    json_reader_read_element(jsondata_reader,i); //读取第i个元素
}

int8_t draw::jsondata_element_out()
{
    json_reader_end_element(jsondata_reader); //返回上一个节点
}

int8_t draw::draw_rectangle(color argb, double x, double y, double width, double height)
{
    if(!this->initrd)return 1;

    cairo_save(cr);//保存画笔
    cairo_set_source_rgba (cr, argb.red_double(), argb.green_double(), argb.blue_double(), argb.alpha_double());
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);
    cairo_restore(cr);//还原画笔

    return 0;
}

int8_t draw::draw_text(const char *text, const char *family, double font_size, char alignment, color argb, double x, double y)
{
    if(!this->initrd)return 1;

    cairo_save(cr);//保存画笔
    cairo_set_source_rgba (cr, argb.red_double(), argb.green_double(), argb.blue_double(), argb.alpha_double());
    cairo_select_font_face (cr, family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, font_size);

    if (alignment)
    {
        cairo_text_extents_t extents;
        cairo_text_extents(cr,text,&extents);
        cairo_move_to (cr, x-extents.width/2, y);
    }
    else
    {
        cairo_move_to (cr, x, y);
    }

    //cairo_show_text (cr, text);
    cairo_text_path (cr, text);
    cairo_fill(cr);
    cairo_restore(cr);//还原画笔

    return 0;
}

int8_t draw::draw_png (const char *pngfilename, double x, double y, double width, double height)
{
    if(!this->initrd)return 1;
    if(!this->filecheck(pngfilename))return 2;

    cairo_surface_t *img=NULL;
    img=cairo_image_surface_create_from_png(pngfilename);

    cairo_save(cr);//保存画笔

    cairo_translate (cr, x, y);
    if(width||height)
    {
        unsigned int png_width, png_height;
        double scaleX, scaleY;
        png_width=cairo_image_surface_get_width(img);
        png_height=cairo_image_surface_get_height(img);
        scaleX=width/(double)png_width;
        scaleY=height/(double)png_height;
        cairo_scale (cr, scaleX, scaleY);
    }
    cairo_set_source_surface(cr,img,0,0);
    cairo_paint(cr);

    cairo_surface_destroy (img);//回收PNG介质
    cairo_restore(cr);//还原画笔

    return 0;
}

int8_t draw::draw_svg (const char *svgfilename, double x, double y, double width, double height)
{
    if(!this->initrd)return 1;
    if(!this->filecheck(svgfilename))return 2;

    RsvgHandle *svg;
    svg = rsvg_handle_new_from_file(svgfilename,NULL);

    cairo_save(cr);//保存画笔

    cairo_translate (cr, x, y);
    if(width||height)
    {
        unsigned int svg_width, svg_height;
        double scaleX, scaleY;
        RsvgDimensionData dimension_data;
        rsvg_handle_get_dimensions(svg,&dimension_data);
        svg_width=dimension_data.width;
        svg_height=dimension_data.height;
        scaleX=width/(double)svg_width;
        scaleY=height/(double)svg_height;
        cairo_scale (cr, scaleX, scaleY);
    }
    rsvg_handle_render_cairo(svg, cr);

    rsvg_handle_close(svg,NULL);//释放handle
    cairo_restore(cr);//还原画笔

    return 0;
}

int8_t draw::filecheck (const char *filename)
{
    FILE* file;
    file=fopen (filename, "rb");
    if(file)
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}
