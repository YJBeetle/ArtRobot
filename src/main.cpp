#include "default.h"

#include "Args.h"
#include "Color.h"
#include "Draw.h"

int main(int argc, char *argv[])
{
#ifdef TIMER
    // 计时开始
    struct timeval tpstart, tpend;
    float timeuse;
    gettimeofday(&tpstart, NULL);
#endif // TIMER

    // 解析argv
    Args args(argc, argv);

    // 解析Json
    Json json;
    ifstream jsonFile(args.jsonfile, ifstream::in);
    jsonFile >> json;

    // 读取文档参数
    double width = 200; // 默认200
    {
        auto widthJson = json.find("w");
        if (widthJson != json.end())
        { // Has "width"
            width = *widthJson;
        }
    }
    double height = 200; // 默认200
    {
        auto heightJson = json.find("h");
        if (heightJson != json.end())
        { // Has "height"
            height = *heightJson;
        }
    }
    Draw::unitType unit = Draw::PX; // 默认px
    {
        auto unitJson = json.find("unit");
        if (unitJson != json.end())
        { // Has "unit"
            string unitStr = *unitJson;
            if (unitStr == "px" || unitStr == "pt")
                unit = Draw::PX;
            else if (unitStr == "in" || unitStr == "inch")
                unit = Draw::IN;
            else if (unitStr == "mm")
                unit = Draw::MM;
            else if (unitStr == "cm")
                unit = Draw::CM;
        }
    }
    double ppi = 72; // 默认72
    {
        auto ppiJson = json.find("ppi");
        if (ppiJson != json.end())
        { // Has "ppi"
            ppi = *ppiJson;
        }
    }

    //准备绘制
    Draw draw(args.output, args.type, width, height, unit, ppi);
    int64_t page_count = json["draw"].size();
    int64_t page_i = 0;
    int64_t layer_count = 0;
    int64_t layer_i = 0;

    string layer_type;
    for (page_i = 0; page_i < page_count; page_i++)
    {
        auto nowPage = json["draw"][page_i];
        layer_count = nowPage.size();                       //元素个数
        for (layer_i = 0; layer_i < layer_count; layer_i++) //循环处理该成员中的元素
        {
            auto layer = nowPage[layer_i];

            layer_type = layer["type"];
            if (layer_type == "rectangle")
            {
                draw.draw_rectangle(string(layer["color"]).c_str(), layer["x"], layer["y"], layer["width"], layer["height"]);
            }
            else if (layer_type == "text")
            {
                draw.draw_text(layer["text"], layer["font"], layer["face"], layer["size"], layer["alignment"], string(layer["color"]).c_str(), layer["x"], layer["y"]);
            }
            else if (layer_type == "svgfile")
            {
                draw.draw_svg(layer["filename"], layer["x"], layer["y"], layer["width"], layer["height"]);
            }
            else if (layer_type == "pngfile")
            {
                draw.draw_png(layer["filename"], layer["x"], layer["y"], layer["width"], layer["height"]);
            }
        }

        if (page_i + 1 < page_count)
            draw.nextpage();
    }

#ifdef TIMER
    //计时结束
    gettimeofday(&tpend, NULL);
    timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
    timeuse /= 1000000;
    //输出耗时
    fprintf(stderr, "Used Time: %fs\n", timeuse);
#endif // TIMER

    return 0;
}
