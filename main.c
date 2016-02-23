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
    page_width=A4_HEIGHT;
    page_height=A4_WIDTH;

    draw("out.pdf",1);
    draw("out.svg",2);

    printf("over!\n");
    return 0;
}
