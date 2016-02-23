#include <stdio.h>
#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <svg-cairo.h>
#include <math.h>
#include <string.h>

#include "draw.h"




/* This path desribes what will be drawn later
   The bulk of it is doing the IBM letters by connect the dots
   at the very end, we will get fancy and add a (R) Registered logo.
*/
void travel_path (cairo_t *cr)
{
   int     pen_radius         = 10;

   cairo_set_source_rgb (cr, 1,1,1);
   cairo_paint (cr);

   cairo_set_line_width (cr, pen_radius*2);

   /* Use IBM Blue Pen Color with no ALPHA */
   cairo_set_source_rgba (cr, .3, .42, .69, 1);


   /* This Draws the IBM 8 Lines "I" Logo */
   cairo_move_to (cr, 10, 10);
   cairo_line_to (cr, 160, 10);

   cairo_move_to (cr, 10, 40);
   cairo_line_to (cr, 160, 40);

   /* NOTE: Narrower Middle: 1/3 width */
   cairo_move_to (cr, 60, 70);
   cairo_line_to (cr, 110, 70);

   cairo_move_to (cr, 60, 100);
   cairo_line_to (cr, 110, 100);

   cairo_move_to (cr, 60, 130);
   cairo_line_to (cr, 110, 130);

   cairo_move_to (cr, 60, 160);
   cairo_line_to (cr, 110, 160);
   /* END Narrower Middle */

   cairo_move_to (cr, 10, 190);
   cairo_line_to (cr, 160, 190);

   cairo_move_to (cr, 10, 220);
   cairo_line_to (cr, 160, 220);
   /* END "I" DRAWING */




   /* This Draws the IBM 8 Lines "B" Logo */
   cairo_move_to (cr, 170, 10);
   cairo_line_to (cr, 340, 10);

   cairo_move_to (cr, 170, 40);
   cairo_line_to (cr, 360, 40);

   cairo_move_to (cr, 200, 70);
   cairo_line_to (cr, 250, 70);
   /*B's have holes in them! */
   cairo_move_to (cr, 300, 70);
   cairo_line_to (cr, 360, 70);


   cairo_move_to (cr, 210, 100);
   cairo_line_to (cr, 350, 100);


   cairo_move_to (cr, 210, 130);
   cairo_line_to (cr, 350, 130);


   cairo_move_to (cr, 200, 160);
   cairo_line_to (cr, 250, 160);
   /*B's have holes in them! */
   cairo_move_to (cr, 300, 160);
   cairo_line_to (cr, 360, 160);


   cairo_move_to (cr, 170, 190);
   cairo_line_to (cr, 360, 190);


   cairo_move_to (cr, 170, 220);
   cairo_line_to (cr, 340, 220);
   /* END "B" DRAWING */




   /* THE EVER POINTY "M"  */
   cairo_move_to (cr, 370, 10);
   cairo_line_to (cr, 470, 10);
   cairo_move_to (cr, 560, 10);
   cairo_line_to (cr, 660, 10);


   cairo_move_to (cr, 370, 40);
   cairo_line_to (cr, 490, 40);
   cairo_move_to (cr, 540, 40);
   cairo_line_to (cr, 660, 40);


   cairo_move_to (cr, 400, 70);
   cairo_line_to (cr, 510, 70);
   cairo_move_to (cr, 520, 70);
   cairo_line_to (cr, 630, 70);


   cairo_move_to (cr, 400, 100);
   cairo_line_to (cr, 630, 100);


   cairo_move_to (cr, 400, 130);
   cairo_line_to (cr, 470, 130);
   cairo_move_to (cr, 480, 130);
   cairo_line_to (cr, 550, 130);
   cairo_move_to (cr, 560, 130);
   cairo_line_to (cr, 630, 130);


   cairo_move_to (cr, 400, 160);
   cairo_line_to (cr, 470, 160);
   cairo_move_to (cr, 490, 160);
   cairo_line_to (cr, 540, 160);
   cairo_move_to (cr, 560, 160);
   cairo_line_to (cr, 630, 160);


   cairo_move_to (cr, 370, 190);
   cairo_line_to (cr, 470, 190);
   cairo_move_to (cr, 500, 190);
   cairo_line_to (cr, 530, 190);
   cairo_move_to (cr, 560, 190);
   cairo_line_to (cr, 660, 190);


   cairo_move_to (cr, 370, 220);
   cairo_line_to (cr, 470, 220);
   cairo_move_to (cr, 510, 220);
   cairo_line_to (cr, 520, 220);
   cairo_move_to (cr, 560, 220);
   cairo_line_to (cr, 660, 220);

   /* END POINTY LETTERS */

   /* We stroke the path so we see everything we just specified
      by connecting the dots
   */
   cairo_stroke(cr);




   /* Let us add a disclaimer and show some fancy cairo: */
   /* We are going to want a nice fine lined circle around the R
   you need to make sure you have stroked existing things
   that you wanted drawn with the larger pen before continuing.
   */
   cairo_set_line_width (cr, pen_radius*.5);

   /* Now we will draw the fancy circle around the "R" */
   /* NOTE: The angles are in radians */
   cairo_move_to (cr, 710, 200);
   double angle1 = 0 * (M_PI/180.0);
   double angle2 = 360 * (M_PI/180.0);

   /* We draw a large black circle */
   cairo_set_source_rgba (cr, 0, 0, 0, 1);
   cairo_arc (cr, 710, 200, 20, angle1, angle2);
   cairo_stroke (cr);

   /* We draw a smaller white circle centered on it */
   cairo_set_source_rgba (cr, 1, 1, 1, 1);
   cairo_arc (cr, 710, 200, 20, angle1, angle2);
   /* We use the fill operator to fill in the circle! */
   cairo_fill (cr);

   /* We are going to draw the letter "R" with black pen*/

   cairo_move_to (cr, 695,212); /* Bottom left of text at point */
   cairo_set_source_rgba (cr, 0, 0, 0, 1);
   cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                                       CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size (cr, 40);
   cairo_show_text (cr, "R");

    /* We stroke everything we have just done
       to actually draw it...
    */
    cairo_stroke (cr);

}

void drawsvg (cairo_t *cr, char *svgfilename, unsigned int width, unsigned int height)
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

    //设置画笔
    svg_cairo_get_size (svgc, &svg_width, &svg_height);//取得svg大小
    printf("svg size: %d, %d\n",svg_width, svg_height);
    printf("svg to size: %d, %d\n",width, height);
    scaleX=(double)width/(double)svg_width;
    scaleY=(double)height/(double)svg_height;

    printf("scaleXY: %f, %f\n",scaleX, scaleY);

    //cairo_translate (cr, 100, 100);
    cairo_scale (cr, scaleX, scaleY);
    //cairo_set_source_rgb (cr, 1, 1, 1);

    svg_cairo_render (svgc, cr);//绘制

    svg_cairo_destroy(svgc);//释放
    fclose (svgfile);//关闭
    cairo_restore(cr);//还原画笔
}

void drawtext (cairo_t *cr,char *text)
{
    cairo_save(cr);//保存画笔
    cairo_move_to (cr, 40, 40); /* Bottom left of text at point */
    cairo_set_source_rgba (cr, 0, 0, 0, 1);
    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 40);
    cairo_show_text (cr, text);
    cairo_restore(cr);//还原画笔
}

struct color color_code2rgba(int code)
{
    struct color argb;
    if(code<=0xffffff)
        argb.alpha=(double)1;
    else
        argb.alpha=(double)((code&0xff000000)>>6)/(double)0xff;
    argb.red=(double)((code&0xff0000)>>4)/(double)0xff;
    argb.green=(double)((code&0xff00)>>2)/(double)0xff;
    argb.blue=(double)(code&0xff)/(double)0xff;
    return argb;
}

void draw_rectangle(cairo_t *cr, double x, double y, double width, double height,int color_code)
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
    //travel_path (cr);//IBM
    draw_rectangle(cr, 0, 0, A4_WIDTH, A4_HEIGHT,0xFCF7E8);
    drawsvg(cr,"test.svg", A4_WIDTH, A4_HEIGHT);
    drawtext(cr,"YJBeetle");

    cairo_destroy (cr);//回收画笔
    cairo_surface_destroy (surface);//回收介质
}


