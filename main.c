#include <stdio.h>
#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <svg-cairo.h>
#include <math.h>
#include <string.h>

#include "draw.h"

int main(int argc, char *argv[])
{
    cairo_surface_t *surface;

    surface = cairo_pdf_surface_create ("out.pdf", MM2PT(A4_WIDTH), MM2PT(A4_HEIGHT));
    //cairo_surface_set_fallback_resolution(surface,300,300);//设置分辨率
    draw (surface);
    cairo_surface_destroy (surface);//回收

/*
    surface = cairo_svg_surface_create ("out.svg", A4_WIDTH, A4_HEIGHT);
    draw (surface);
    cairo_surface_destroy (surface);
*/

    printf("over!\n");
    return 0;
}
