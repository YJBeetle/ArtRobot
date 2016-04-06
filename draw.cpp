#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <cairo-ft.h>
#include <librsvg/rsvg.h>
#include <json-glib/json-glib.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TYPE1_TABLES_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H

#include "default.h"
#include "color.h"
#include "json.h"
#include "draw.h"

draw::draw()
{
    this->inited=0;
}

int8_t draw::make(const char *jsondata)
{
    if(!jsondata)return 1;
    json.init(jsondata);

    switch(init(json.get_string("outfile"),json.get_string("type"),json.get_double("width"),json.get_double("height")))
    {
    case 0:
        break;
    case 1:
        fprintf(stderr,"Init: warning: Repeat initialize!\n");
        break;
    case 2:
        fprintf(stderr,"Init: error: outfile is unavailable , Failure to initialize!\n");
        break;
    case 3:
        fprintf(stderr,"Init: error: Unknow type , Failure to initialize!\n");
        break;
    default:
        fprintf(stderr,"Init: error: Unknow error code\n");
    }

    json.read_member("draw");
    int count=json.count(); //元素个数
    const char *type;
    for(int i=0;json.read_element(i),i<count;json.end_element(),++i) //循环处理该成员中的元素
    {
        type=json.get_string("type");

        if(strstr(type,"rectangle"))
        {
            switch(draw_rectangle(json.get_string("color"), json.get_double("x"), json.get_double("y"), json.get_double("width"), json.get_double("height")))
            {
            case 0:
                break;
            case 1:
                fprintf(stderr,"DrawRectangle: warning: not initialized!\n");
                break;
            default:
                fprintf(stderr,"DrawRectangle: warning: Unknow error code.\n");
            }
        }
        else if(strstr(type,"text"))
        {
            switch(draw_text(json.get_string("text"), json.get_string("family"), json.get_double("size"), json.get_int("alignment"), json.get_string("color"), json.get_double("x"), json.get_double("y")))
            {
            case 0:
                break;
            case 1:
                fprintf(stderr,"DrawTEXT: warning: not initialized!\n");
                break;
            case 2:
                fprintf(stderr,"DrawTEXT: warning: no text.\n");
                break;
            case 3:
                fprintf(stderr,"DrawTEXT: warning: no family.\n");
                break;
            default:
                fprintf(stderr,"DrawTEXT: warning: Unknow error code.\n");
            }
        }
        else if(strstr(type,"svgfile"))
        {
            switch(draw_svg(json.get_string("filename"), json.get_double("x"), json.get_double("y"), json.get_double("width"), json.get_double("height")))
            {
            case 0:
                break;
            case 1:
                fprintf(stderr,"DrawSVG: warning: not initialized!\n");
                break;
            case 2:
                fprintf(stderr,"DrawSVG: warning: file not found: %s\n",json.get_string("filename"));
                break;
            default:
                fprintf(stderr,"DrawSVG: warning: Unknow error code.\n");
            }
        }
        else if(strstr(type,"pngfile"))
        {
            switch(draw_png(json.get_string("filename"), json.get_double("x"), json.get_double("y"), json.get_double("width"), json.get_double("height")))
            {
            case 0:
                break;
            case 1:
                fprintf(stderr,"DrawPNG: warning: not initialized!\n");
                break;
            case 2:
                fprintf(stderr,"DrawPNG: warning: file not found: %s\n",json.get_string("filename"));
                break;
            default:
                fprintf(stderr,"DrawPNG: warning: Unknow error code.\n");
            }
        }
    }

    uninit();

    return 0;
}

int8_t draw::init(const char *filename,const char *type,double width,double height)
{
    if(this->inited)return 1;
    if(!filename)return 2;
    if(!type)return 3;

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
        this->inited=0;
        return 3;
    }

    this->inited=1;
    return 0;
}

int8_t draw::uninit()
{
    if(!this->inited)return 1;

    cairo_destroy (cr);//回收画笔
    cairo_surface_destroy (surface);//回收介质

    return 0;
}

int8_t draw::draw_rectangle(Color argb, double x, double y, double width, double height)
{
    if(!this->inited)return 1;

    cairo_save(cr);//保存画笔
    cairo_set_source_rgba (cr, argb.redDouble(), argb.greenDouble(), argb.blueDouble(), argb.alphaDouble());
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);
    cairo_restore(cr);//还原画笔

    return 0;
}

int8_t draw::draw_text(const char *text, const char *fontfile, double font_size, int8_t alignment, Color argb, double x, double y)
{
    if(!this->inited)return 1;
    if(!text)return 2;
    if(!fontfile)return 3;

    cairo_save(cr);//保存画笔
    FT_Library ft_library;
    FT_Face ft_face;
    cairo_font_face_t *cr_face;
    if (FT_Init_FreeType (&ft_library)) {
        printf("FT_Init_FreeType failed\n");
        return -1;
    }
    if (FT_New_Face (ft_library, fontfile, 0, &ft_face)) {
        printf("FT_New_Face failed\n");
        return -1;
    }
    cr_face = cairo_ft_font_face_create_for_ft_face (ft_face, 0);
    cairo_set_font_face (cr, cr_face);
    //cairo_select_font_face (cr, family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, font_size);
    cairo_set_source_rgba (cr, argb.redDouble(), argb.greenDouble(), argb.blueDouble(), argb.alphaDouble());

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

int8_t draw::draw_svg (const char *svgfilename, double x, double y, double width, double height)
{
    if(!this->inited)return 1;
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

int8_t draw::draw_png (const char *pngfilename, double x, double y, double width, double height)
{
    if(!this->inited)return 1;
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
