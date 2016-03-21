#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <librsvg/rsvg.h>

#include "default.h"
#include "color.h"
#include "draw.h"

#include <sys/time.h>

int main(int argc, char *argv[])
{
    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);



    //char *json='[{"type":"rectangle","color_code":"FCF7E8"},{"type":"svg","filename":"bg-veins.svg"},{"type":"png","filename":"1.png"},{"type":"text","text":"YJBeetle"}]';


    draw pdffile;
    pdffile.make();




    gettimeofday(&tpend,NULL);
    timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
    tpend.tv_usec-tpstart.tv_usec;
    timeuse/=1000000;
    printf("Used Time:%f\n",timeuse);


    printf("over!\n");
    return 0;
}
