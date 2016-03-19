#include <stdio.h>
#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo/cairo-ps.h>
#include <cairo-svg.h>
//#include <svg-cairo.h>
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>
#include <math.h>
#include <string.h>

#include "draw.h"


struct color color_code2rgba(int32_t code)
{
    struct color argb;
    if(code<=0xffffff)
        argb.alpha=(double)1;
    else
        argb.alpha=(double)(code>>24&0xff)/(double)0xff;
    argb.red=   (double)(code>>16&0xff)/(double)0xff;
    argb.green= (double)(code>>8&0xff)/(double)0xff;
    argb.blue=  (double)(code&0xff)/(double)0xff;
    return argb;
}

void draw_fromsvg (cairo_t *cr, char *svgfilename, double x, double y, double width, double height)
{
/*
    FILE *svgfile;
    svg_cairo_t *svgc;

    svgfile=fopen (svgfilename, "r");//打开文件
    svg_cairo_create (&svgc);//创建svgc对象
    if ( svg_cairo_parse_file (svgc, svgfile) )//读取文件
    {
        fprintf (stderr, "无法读取SVG\n");
        svg_cairo_destroy(svgc);//释放
        fclose (svgfile);//关闭
        return;
    }

    cairo_save(cr);//保存画笔

    //设置绘画位置大小
    cairo_translate (cr, x, y);
    if(width||height)
    {
        unsigned int svg_width, svg_height;
        double scaleX, scaleY;
        svg_cairo_get_size (svgc, &svg_width, &svg_height);//取得svg大小
        scaleX=width/(double)svg_width;
        scaleY=height/(double)svg_height;
        cairo_scale (cr, scaleX, scaleY);
    }

    //cairo_set_source_rgb (cr, 1, 1, 1);
    svg_cairo_render (svgc, cr);//绘制

    svg_cairo_destroy(svgc);//释放
    fclose (svgfile);//关闭
    cairo_restore(cr);//还原画笔
*/
    RsvgHandle *handle;
    handle = rsvg_handle_new_from_file(svgfilename,NULL);

    cairo_save(cr);//保存画笔

    cairo_translate (cr, x, y);
    if(width||height)
    {
        unsigned int svg_width, svg_height;
        double scaleX, scaleY;
        RsvgDimensionData dimension_data;
        rsvg_handle_get_dimensions(handle,&dimension_data);
printf("w:%dh:%d\n",dimension_data.width,dimension_data.height);
        svg_width=dimension_data.width;
        svg_height=dimension_data.height;
        scaleX=width/(double)svg_width;
        scaleY=height/(double)svg_height;
        cairo_scale (cr, scaleX, scaleY);
    }
    rsvg_handle_render_cairo(handle, cr);

    cairo_restore(cr);//还原画笔
}

void draw_frompng (cairo_t *cr, char *pngfilename, double x, double y, double width, double height)
{
    cairo_surface_t *img;
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
}

void draw_text (cairo_t *cr,char *text, char *family, double font_size, char alignment, int32_t color_code, double x, double y)
{
    cairo_save(cr);//保存画笔
    struct color argb=color_code2rgba(color_code);
    cairo_set_source_rgba (cr, argb.red, argb.green, argb.blue, argb.alpha);
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
}

void draw_rectangle(cairo_t *cr, int32_t color_code, double x, double y, double width, double height)
{
    cairo_save(cr);//保存画笔
    struct color argb=color_code2rgba(color_code);
    cairo_set_source_rgba (cr, argb.red, argb.green, argb.blue, argb.alpha);
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);
    cairo_restore(cr);//还原画笔
}

void draw (char *outfile)
{
    cairo_surface_t *surface;//介质
    cairo_t *cr;//画笔

    switch (surface_type) {
    case 1://PDF
        surface = cairo_pdf_surface_create (outfile, MM2PT(page_width), MM2PT(page_height));//创建介质
        //cairo_surface_set_fallback_resolution(surface,300,300);//设置分辨率
        cr = cairo_create (surface);//创建画笔
        cairo_scale (cr, MM2PT(1), MM2PT(1));//缩放画笔，因PDF用mm作为最终单位故需缩放画笔
        break;
    case 2://SVG
        surface = cairo_svg_surface_create (outfile, page_width, page_height);
        cr = cairo_create (surface);//创建画笔
        break;
    default:
        fprintf (stderr, "未知的输出类型\n");
        return;
        break;
    }

    //travel_path (cr);//IBM
    //cairo_show_page(cr);//换页
    //cairo_rotate (cr, -M_PI / 4);//旋转画笔
    //cairo_scale (cr, 2, 1.0);//缩放画笔











    draw_rectangle(cr, 0xFCF7E8, 0, 0, page_width, page_height);
    draw_fromsvg(cr,"bg-veins.svg", 0, 0, page_width, page_height);
    //draw_frompng(cr,"1.png",100,100,100,100);
    draw_fromsvg(cr,"logo.svg", (page_width-50)/2, 30, 50, 37.544);
    draw_rectangle(cr, 0x686767, 0, 0, page_width, 16);
    draw_rectangle(cr, 0x686767, 0, page_height-16, page_width, 16);
    draw_text(cr,"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an", "Lantinghei SC Demibold", 5, 1, 0xffffff, page_width/2, 10);
    draw_text(cr,"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an", "Lantinghei SC Demibold", 5, 1, 0xffffff, page_width/2, page_height-6);
    draw_text(cr,"YJBeetle", "Yuanti SC", 20, 1, 0x686767, page_width/2, 100);
    draw_text(cr,"A测试中文", "Lantinghei SC Extralight", 20, 1, 0x686767, page_width/2, 120);









    cairo_destroy (cr);//回收画笔
    cairo_surface_destroy (surface);//回收介质
}


