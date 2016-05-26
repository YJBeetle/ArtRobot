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
#include "color.h"
#include "json.h"
#include "draw.h"

#include <sys/time.h>

int main(int argc, char *argv[])
{
    const char *jsondata=NULL;

    //解析argv
    const char *argv1=NULL;
    argv1=argv[1]?argv[1]:"";
    if(strcmp(argv1,"-f")==0)
    {
        FILE* jsonfile;
        jsonfile=fopen(argv[2],"r");
        if(jsonfile)
        {
            char *d;
            fseek(jsonfile,0,SEEK_END);
            int len=ftell(jsonfile);
            d=new char[len+1];
            rewind(jsonfile);
            fread(d,1,len,jsonfile);
            d[len]=0;
            fclose(jsonfile);
            jsondata=d;
        }
        else
        {
            printf("文件不存在\n");
            return 1;
        }
    }
//    else if(strcmp(argv1,"-c")==0)
//    {
//        char *line = NULL;
//        size_t len = 0;
//        ssize_t read;
//
//        while ((read = getline(&line, &len, stdin)) != -1)
//        {
//            printf("Retrieved line of length %zu, %u :\n", read, len);
//            printf("%s", line);
//        }
//    }
    else if(strcmp(argv1,"-h")==0||strcmp(argv1,"--help")==0)
    {
        printf("帮助：\n  -c        stdin输入\n  -f [file] 文件输入\n  -h        显示本帮助信息\n");
        return 0;
    }
    else
    {
        jsondata=argv[1];
    }

    //测试用
    //jsondata="{\"outfile\":\"out.svg\",\"type\":\"SVG\",\"width\":420,\"height\":297,\"draw\":[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"img/bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an\",\"font\":\"fonts/Lantinghei.ttc\",\"face\":2,\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"img/logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"会议名称\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"会议名称行2\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":26,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":130},{\"type\":\"text\",\"text\":\"会议时间\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":170},{\"type\":\"text\",\"text\":\"会议地点\",\"font\":\"fonts/Yuanti.ttc\",\"face\":2,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":190},{\"type\":\"text\",\"text\":\"Date\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":220},{\"type\":\"text\",\"text\":\"Location\",\"font\":\"fonts/Yuanti.ttc\",\"face\":1,\"size\":18,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":240}]}";

    //计时开始
    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);

    //do
    draw drawfile;
    drawfile.make(jsondata);

    //计时结束
    gettimeofday(&tpend,NULL);
    timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    timeuse/=1000000;

    //输出耗时
    printf("Used Time:%fs\n",timeuse);

    return 0;
}
