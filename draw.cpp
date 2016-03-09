#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <svg-cairo.h>

#include "default.h"
#include "color.h"
#include "draw.h"

draw::draw()
{
    this->initrd=0;
}

void draw::init(char *filename,_surface_type type,double width,double height)
{
    if(!this->initrd)
        return;
    this->initrd=1;

    this->outfile=filename;
    this->surface_type=type;
    this->page_width=width;
    this->page_height=height;

    switch (surface_type) {
    case PDF://PDF
        surface = cairo_pdf_surface_create (outfile, MM2PT(page_width), MM2PT(page_height));//创建介质
        //cairo_surface_set_fallback_resolution(surface,300,300);//设置分辨率
        cr = cairo_create (surface);//创建画笔
        cairo_scale (cr, MM2PT(1), MM2PT(1));//缩放画笔，因PDF用mm作为最终单位故需缩放画笔
        break;
    case SVG://SVG
        surface = cairo_svg_surface_create (outfile, page_width, page_height);
        cr = cairo_create (surface);//创建画笔
        break;
    default:
        fprintf (stderr, "未知的输出类型\n");
        this->initrd=0;
        return;
        break;
    }
}

int8_t draw::make()
{
    init("out.pdf",PDF,A3_HEIGHT,A3_WIDTH);


    //travel_path (cr);//IBM
    //cairo_show_page(cr);//换页
    //cairo_rotate (cr, -M_PI / 4);//旋转画笔
    //cairo_scale (cr, 2, 1.0);//缩放画笔


    draw_rectangle(0xFCF7E8, 0, 0, page_width, page_height);
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
}

void draw::draw_rectangle(int32_t color_code, double x, double y, double width, double height)
{
    cairo_save(cr);//保存画笔
    color argb(color_code);
    cairo_set_source_rgba (cr, argb.red_double(), argb.green_double(), argb.blue_double(), argb.alpha_double());
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);
    cairo_restore(cr);//还原画笔
}

void draw::draw_text(char *text, char *family, double font_size, char alignment, int32_t color_code, double x, double y)
{
    cairo_save(cr);//保存画笔
    color argb(color_code);
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
}

void draw::draw_png (char *pngfilename, double x, double y, double width, double height)
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

void draw::draw_svg (char *svgfilename, double x, double y, double width, double height)
{
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
}



