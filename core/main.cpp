#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <cairo-ft.h>
#include <librsvg/rsvg.h>
#include <json-glib/json-glib.h>

#include "default.h"
#include "Args.h"
#include "Color.h"
#include "Json.h"
#include "Draw.h"

#include <sys/time.h>

int main(int argc, char *argv[])
{
#ifdef DEBUG
    //计时开始
    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);
#endif

    //解析argv
    Args args;
    args.args_parse(argc,argv);

    //解析Json
    Json json;
    if(json.init(args.jsondata_s()))
    {
#ifdef DEBUG
        fprintf(stderr,"Main: error: Json Init failed!\n");
#endif
        return 1;
    }

    //绘制
    Draw draw;
    if(draw.init(args.output_s(),json.get_string("type"),json.get_double("width"),json.get_double("height")))
    {
#ifdef DEBUG
        fprintf(stderr,"Main: error: Draw Init failed!\n");
#endif
        return 2;
    }

    //准备绘制，默认多页
    json.read_member("draw");
    int8_t multipage=1;
    int64_t page_count=json.count();
    int64_t page_i=0;
    int64_t layer_count=0;
    int64_t layer_i=0;

    //单页检测，如果是单页
    json.read_element(0);
    if(json.get_string("type"))
    {
        multipage=0;
        page_count=1;
    }
    json.end_element();

    const char *layer_type;
    for(page_i=0;page_i<page_count;page_i++)
    {
        if(multipage)json.read_element(page_i);

        layer_count=json.count(); //元素个数
        for(layer_i=0;layer_i<layer_count;layer_i++) //循环处理该成员中的元素
        {
            json.read_element(layer_i);

            layer_type=json.get_string("type");
            if(!layer_type)continue;
            if(!strcasecmp(layer_type,"rectangle"))
            {
                draw.draw_rectangle(json.get_string("color"), json.get_double("x"), json.get_double("y"), json.get_double("width"), json.get_double("height"));
            }
            else if(!strcasecmp(layer_type,"text"))
            {
                draw.draw_text(json.get_string("text"), json.get_string("font"), json.get_int("face"), json.get_double("size"), json.get_int("alignment"), json.get_string("color"), json.get_double("x"), json.get_double("y"));
            }
            else if(!strcasecmp(layer_type,"svgfile"))
            {
                draw.draw_svg(json.get_string("filename"), json.get_double("x"), json.get_double("y"), json.get_double("width"), json.get_double("height"));
            }
            else if(!strcasecmp(layer_type,"pngfile"))
            {
                draw.draw_png(json.get_string("filename"), json.get_double("x"), json.get_double("y"), json.get_double("width"), json.get_double("height"));
            }

            json.end_element();
        }

        if(multipage)
        {
            json.end_element();
            if(page_i+1<page_count)draw.nextpage();
        }
    }

    draw.uninit();


#ifdef DEBUG
    //计时结束
    gettimeofday(&tpend,NULL);
    timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=1000000;
    //输出耗时
    fprintf(stderr,"Used Time: %fs\n",timeuse);
#endif

    return 0;
}
