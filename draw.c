#include <stdio.h>
#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <svg-cairo.h>
#include <math.h>
#include <string.h>

#include "draw.h"


void drawsvg (cairo_t *cr, char *svgfilename, double x, double y, double width, double height)
{
    FILE *svgfile;
    svg_cairo_t *svgc;
    unsigned int svg_width, svg_height;
    double scaleX, scaleY;

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
    svg_cairo_get_size (svgc, &svg_width, &svg_height);//取得svg大小
printf("svg:[%s] size:[%d],[%d]\n",svgfilename,svg_width,svg_height);
    scaleX=width/(double)svg_width;
    scaleY=height/(double)svg_height;
    cairo_scale (cr, scaleX, scaleY);

    //cairo_set_source_rgb (cr, 1, 1, 1);
    svg_cairo_render (svgc, cr);//绘制

    svg_cairo_destroy(svgc);//释放
    fclose (svgfile);//关闭
    cairo_restore(cr);//还原画笔
}

void drawtext (cairo_t *cr,char *text, double x, double y)
{
    cairo_save(cr);//保存画笔
    cairo_move_to (cr, 40, 40); /* Bottom left of text at point */
    cairo_set_source_rgba (cr, 0, 0, 0, 1);
    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 10);
    cairo_show_text (cr, text);
    cairo_restore(cr);//还原画笔
}

struct color color_code2rgba(int code)
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

void draw_rectangle(cairo_t *cr, int color_code, double x, double y, double width, double height)
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
    drawsvg(cr,"bg-veins.svg", 0, 0, page_width, page_height);
    draw_rectangle(cr, 0x686767, 0, 0, page_width, 16);
    draw_rectangle(cr, 0x686767, 0, page_height-16, page_width, 16);
    drawsvg(cr,"logo.svg", (page_width-50)/2, 30, 50, 37.544);
    drawtext(cr,"YJBeetle", 0, 0);

    cairo_destroy (cr);//回收画笔
    cairo_surface_destroy (surface);//回收介质
}


