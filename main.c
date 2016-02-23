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
    page_width=A3_HEIGHT;
    page_height=A3_WIDTH;

    surface_type=1;
    draw("out.pdf");

    surface_type=2;
    draw("out.svg");

    printf("over!\n");
    return 0;
}
