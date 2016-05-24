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
    //jsondata="{\"outfile\":\"out.pdf\",\"type\":\"PDF\",\"width\":420,\"height\":297,\"draw\":[{\"type\":\"rectangle\",\"color\":\"FCF7E8\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"svgfile\",\"filename\":\"bg-veins.svg\",\"x\":0,\"y\":0,\"width\":420,\"height\":297},{\"type\":\"pngfile\",\"filename\":\"1.png\",\"x\":10,\"y\":10,\"width\":100,\"height\":100},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":0,\"width\":420,\"height\":16},{\"type\":\"rectangle\",\"color\":\"686767\",\"x\":0,\"y\":281,\"width\":420,\"height\":16},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi\'an\",\"family\":\"Lantinghei SC Demibold\",\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":10},{\"type\":\"text\",\"text\":\"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi\'an\",\"family\":\"Lantinghei SC Demibold\",\"size\":5,\"alignment\":1,\"color\":\"ffffff\",\"x\":210,\"y\":291},{\"type\":\"svgfile\",\"filename\":\"logo.svg\",\"x\":185,\"y\":30,\"width\":50,\"height\":37.544},{\"type\":\"text\",\"text\":\"YJBeetle\",\"family\":\"Yuanti SC\",\"size\":20,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":100},{\"type\":\"text\",\"text\":\"\\u6d4b\\u8bd5\\u4e2d\\u6587\",\"family\":\"Lantinghei SC Extralight\",\"size\":20,\"alignment\":1,\"color\":\"686767\",\"x\":210,\"y\":120}]}";

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
            fseek(jsonfile,0,SEEK_END); //把指针移动到文件的结尾 ，获取文件长度
            int len=ftell(jsonfile); //获取文件长度
            d=new char[len+1]; //定义数组长度
            rewind(jsonfile); //把指针移动到文件开头 因为我们一开始把指针移动到结尾，如果不移动回来 会出错
            fread(d,1,len,jsonfile); //读文件
            d[len]=0; //把读到的文件最后一位 写为0 要不然系统会一直寻找到0后才结束
            fclose(jsonfile);
            jsondata=d;
        }
        else
        {
            printf("文件不存在\n");
            return 1;
        }
    }
    else if(strcmp(argv1,"-c")==0)
    {
        printf("c");
        return 1;
    }
    else if(strcmp(argv1,"-h")==0)
    {
        printf("帮助：\n  -c        stdin输入\n  -f [file] 文件输入\n  -h        显示本帮助信息\n");
        return 0;
    }
    else
    {
        jsondata=argv[1];
    }

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
