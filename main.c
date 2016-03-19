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

#include <sys/time.h>

int main(int argc, char *argv[])
{
    page_width=A3_HEIGHT;
    page_height=A3_WIDTH;



    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);







    surface_type=1;
    draw("out.pdf");

    surface_type=2;
    draw("out.svg");



    //char *json='[{"type":"rectangle","color_code":"FCF7E8"},{"type":"svg","filename":"bg-veins.svg"},{"type":"png","filename":"1.png"},{"type":"text","text":"YJBeetle"}]';





    gettimeofday(&tpend,NULL);
    timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
    tpend.tv_usec-tpstart.tv_usec;
    timeuse/=1000000;
    printf("Used Time:%f\n",timeuse);



    printf("over!\n");
    return 0;
}
