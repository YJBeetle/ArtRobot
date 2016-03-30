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
    this->jsondata_initrd=0;
}

draw::draw(const char *jsondata)
{
    draw();//构造函数初始化
    make(jsondata);
}

int8_t draw::make()//将会删除，测试时期使用
{
    const char *jsondata="{\"outfile\":\"out.pdf\",\"type\":\"PDF\",\"width\":420,\"height\":297,\"draw\":[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"pngfile\",\"filename\":\"1.png\",\"x\":10,\"y\":10,\"width\":100,\"height\":100},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi\'an\",\"family\":\"Lantinghei SC Demibold\",\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi\'an\",\"family\":\"Lantinghei SC Demibold\",\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"YJBeetle\",\"family\":\"Yuanti SC\",\"size\":20,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"\\u6d4b\\u8bd5\\u4e2d\\u6587\",\"family\":\"Lantinghei SC Extralight\",\"size\":20,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":120}]}";
    make(jsondata);
}

int8_t draw::make(const char *jsondata)
{
    jsondata_init(jsondata);

    init(jsondata_get_string("outfile"),jsondata_get_string("type"),jsondata_get_double("width"),jsondata_get_double("height"));

    jsondata_read_member("draw");
    int count=jsondata_count(); //元素个数
    const char *type;
    for(int i=0;jsondata_read_element(i),i<count;jsondata_end_element(),++i) //循环处理该成员中的元素
    {
        type=jsondata_get_string("type");

        if(strstr(type,"rectangle"))
        {
            draw_rectangle(jsondata_get_string("color"), jsondata_get_double("x"), jsondata_get_double("y"), jsondata_get_double("width"), jsondata_get_double("height"));
        }
        else if(strstr(type,"text"))
        {
            draw_text(jsondata_get_string("text"), jsondata_get_string("family"), jsondata_get_double("size"), jsondata_get_int("alignment"), jsondata_get_string("color"), jsondata_get_double("x"), jsondata_get_double("y"));
        }
        else if(strstr(type,"svgfile"))
        {
            draw_svg(jsondata_get_string("filename"), jsondata_get_double("x"), jsondata_get_double("y"), jsondata_get_double("width"), jsondata_get_double("height"));
        }
        else if(strstr(type,"pngfile"))
        {
            draw_png(jsondata_get_string("filename"), jsondata_get_double("x"), jsondata_get_double("y"), jsondata_get_double("width"), jsondata_get_double("height"));
        }
    }

    uninit();

    return 0;
}

int8_t draw::init(const char *filename,const char *type,double width,double height)
{
    if(this->initrd)return 1;
    this->initrd=1;

    //this->outfile=filename;
    //this->out_type=type;
    this->page_width=width;
    this->page_height=height;

    if(strstr(type,"PDF")||strstr(type,"pdf"))
    {
        surface = cairo_pdf_surface_create (filename, MM2PT(page_width), MM2PT(page_height));//创建介质
        //cairo_surface_set_fallback_resolution(surface,300,300);//设置分辨率
        cr = cairo_create (surface);//创建画笔
        cairo_scale (cr, MM2PT(1), MM2PT(1));//缩放画笔，因PDF用mm作为最终单位故需缩放画笔
    }
    else if(strstr(type,"SVG")||strstr(type,"svg"))
    {
        surface = cairo_svg_surface_create (filename, page_width, page_height);
        cr = cairo_create (surface);//创建画笔
    }
    else
    {
        fprintf (stderr, "未知的输出类型\n");
        this->initrd=0;
        return 2;
    }

    return 0;
}

int8_t draw::uninit()
{
    cairo_destroy (cr);//回收画笔
    cairo_surface_destroy (surface);//回收介质
}

int8_t draw::jsondata_init(const char *jsondata)
{
    if(this->jsondata_initrd)return 1;
    this->jsondata_initrd=1;

    jsondata_parser=json_parser_new();
    json_parser_load_from_data(jsondata_parser,jsondata,-1,NULL);
    jsondata_node=json_parser_get_root(jsondata_parser); //得到root结点
    jsondata_reader=json_reader_new(jsondata_node); //使用JsonReader来做解析
    return 0;
}

int32_t draw::jsondata_count()
{
    if(!this->jsondata_initrd)return -1;
    return json_reader_count_elements(jsondata_reader);
}

int8_t draw::jsondata_read_member(const char *member)
{
    if(!this->jsondata_initrd)return 1;
    json_reader_read_member(jsondata_reader,member);
}

int8_t draw::jsondata_end_member()
{
    if(!this->jsondata_initrd)return 1;
    json_reader_end_member(jsondata_reader);
}

int8_t draw::jsondata_read_element(int32_t i)
{
    if(!this->jsondata_initrd)return 1;
    json_reader_read_element(jsondata_reader,i); //读取第i个元素
}

int8_t draw::jsondata_end_element()
{
    if(!this->jsondata_initrd)return 1;
    json_reader_end_element(jsondata_reader); //返回上一个节点
}

const char * draw::jsondata_get_string(const char *item)
{
    if(!this->jsondata_initrd)return 0;
    jsondata_read_member(item); //得到该元素中的成员
    const char *value=json_reader_get_string_value(jsondata_reader);
    jsondata_end_member(); //返回上一个节点
    return value;
}

int64_t draw::jsondata_get_int(const char *item)
{
    if(!this->jsondata_initrd)return 0;
    jsondata_read_member(item); //得到该元素中的成员
    int64_t value=json_reader_get_int_value(jsondata_reader);
    jsondata_end_member(); //返回上一个节点
    return value;
}

double draw::jsondata_get_double(const char *item)
{
    if(!this->jsondata_initrd)return 0;
    jsondata_read_member(item); //得到该元素中的成员
    int64_t value=json_reader_get_double_value(jsondata_reader);
    jsondata_end_member(); //返回上一个节点
    return value;
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

int8_t draw::draw_text(const char *text, const char *family, double font_size, int8_t alignment, color argb, double x, double y)
{
    if(!this->initrd)return 1;
    if(!text)return 2;
    if(!family)return 3;

    cairo_save(cr);//保存画笔
    cairo_set_source_rgba (cr, argb.red_double(), argb.green_double(), argb.blue_double(), argb.alpha_double());
    cairo_select_font_face (cr, family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, font_size);

    cairo_text_extents_t extents;
    switch(alignment)
    {
    case 1:
        cairo_text_extents(cr,text,&extents);
        cairo_move_to (cr, x-extents.width/2, y);
        break;
    case 2:
        cairo_text_extents(cr,text,&extents);
        cairo_move_to (cr, x-extents.width, y);
        break;
    case 0:
    default:
        cairo_move_to (cr, x, y);
        break;
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
