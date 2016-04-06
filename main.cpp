#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <librsvg/rsvg.h>
#include <json-glib/json-glib.h>

#include "default.h"
#include "color.h"
#include "json.h"
#include "draw.h"

#include <sys/time.h>

int main(int argc, char *argv[])
{
    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);

    const char *jsondata=NULL;
    //jsondata="{\"outfile\":\"out.pdf\",\"type\":\"PDF\",\"width\":420,\"height\":297,\"draw\":[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"pngfile\",\"filename\":\"1.png\",\"x\":10,\"y\":10,\"width\":100,\"height\":100},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi\'an\",\"family\":\"Lantinghei SC Demibold\",\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi\'an\",\"family\":\"Lantinghei SC Demibold\",\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"YJBeetle\",\"family\":\"Yuanti SC\",\"size\":20,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"\\u6d4b\\u8bd5\\u4e2d\\u6587\",\"family\":\"Lantinghei SC Extralight\",\"size\":20,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":120}]}";
    jsondata=argv[1];

    draw drawfile;
    drawfile.make(jsondata);

    gettimeofday(&tpend,NULL);
    timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=1000000;
    printf("Used Time:%fs\n",timeuse);

    return 0;
}
