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
#include "args.h"
#include "color.h"
#include "json.h"
#include "draw.h"

#include <sys/time.h>

int main(int argc, char *argv[])
{
    //解析argv
    args args;
    args.args_parse(argc,argv);

    //测试用
    //SVG
    //jsondata="{\"outfile\":\"out.svg\",\"type\":\"SVG\",\"width\":420,\"height\":297,\"draw\":[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"img/bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"img/logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"会议名称\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"会议名称行2\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":130},{\"type\":\"text\",\"text\":\"会议时间\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":170},{\"type\":\"text\",\"text\":\"会议地点\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":190},{\"type\":\"text\",\"text\":\"Date\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":220},{\"type\":\"text\",\"text\":\"Location\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":240}]}";
    //SVG(multi-page)
    //jsondata="{\"outfile\":\"out.svg\",\"type\":\"SVG\",\"width\":420,\"height\":297,\"count\":2,\"draw\":[[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"img/bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"img/logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"会议名称\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"会议名称行2\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":130},{\"type\":\"text\",\"text\":\"会议时间\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":170},{\"type\":\"text\",\"text\":\"会议地点\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":190},{\"type\":\"text\",\"text\":\"Date\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":220},{\"type\":\"text\",\"text\":\"Location\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":240}],[{\"type\":\"pngfile\",\"filename\":\"img/1.png\",\"x\":0,\"y\":0,\"width\":420,\"height\":297}]]}";
    //PDF
    //jsondata="{\"outfile\":\"out.pdf\",\"type\":\"PDF\",\"width\":420,\"height\":297,\"draw\":[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"img/bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"img/logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"会议名称\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"会议名称行2\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":130},{\"type\":\"text\",\"text\":\"会议时间\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":170},{\"type\":\"text\",\"text\":\"会议地点\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":190},{\"type\":\"text\",\"text\":\"Date\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":220},{\"type\":\"text\",\"text\":\"Location\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":240}]}";
    //PDF(multi-page)
    //jsondata="{\"outfile\":\"out.pdf\",\"type\":\"PDF\",\"width\":420,\"height\":297,\"count\":2,\"draw\":[[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"img/bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"img/logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"会议名称\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"会议名称行2\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":130},{\"type\":\"text\",\"text\":\"会议时间\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":170},{\"type\":\"text\",\"text\":\"会议地点\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":190},{\"type\":\"text\",\"text\":\"Date\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":220},{\"type\":\"text\",\"text\":\"Location\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":240}],[{\"type\":\"pngfile\",\"filename\":\"img/1.png\",\"x\":0,\"y\":0,\"width\":420,\"height\":297}]]}";
    //PNG(multi-page)
    //jsondata="{\"outfile\":\"out.png\",\"type\":\"PNG\",\"width\":420,\"height\":297,\"count\":2,\"draw\":[[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"img/bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"img/logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"会议名称\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"会议名称行2\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":130},{\"type\":\"text\",\"text\":\"会议时间\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":170},{\"type\":\"text\",\"text\":\"会议地点\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":190},{\"type\":\"text\",\"text\":\"Date\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":220},{\"type\":\"text\",\"text\":\"Location\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":240}],[{\"type\":\"pngfile\",\"filename\":\"img/1.png\",\"x\":0,\"y\":0,\"width\":420,\"height\":297}]]}";

#ifdef DEBUG
    //计时开始
    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);
#endif

    //do
    draw drawfile;
    drawfile.make(args.jsondata_s(),args.output_s());

#ifdef DEBUG
    //计时结束
    gettimeofday(&tpend,NULL);
    timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=1000000;

    //输出耗时
    fprintf(stderr,"Used Time:%fs\n",timeuse);
#endif

    return 0;
}
