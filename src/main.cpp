#include "default.h"

#include "Args.h"
#include "Color.h"
#include "Renderer.h"

void render(Renderer &renderer, Json &components)
{
    cout << "components size: " << components.size() << endl;

    for (auto &component : components) // 循环处理该成员中的元素
    {
        string type;
        {
            auto typeJson = component.find("type");
            if (typeJson != component.end())
            { // Has "ppi"
                type = *typeJson;
            }
        }

        cout << "type: " << type << endl;

        if (type == "rectangle")
        {
            renderer.draw_rectangle(string(component["color"]).c_str(),
                                    component["x"], component["y"],
                                    component["w"], component["h"]);
        }
        else if (type == "text")
        {
            // renderer.draw_text(component["text"],
            //                    component["font"],
            //                    component["face"],
            //                    component["size"],
            //                    component["alignment"],
            //                    string(component["color"]).c_str(),
            //                    component["x"], component["y"]);
        }
        else if (type == "image")
        {
            renderer.draw_svg(component["src"],
                              component["x"], component["y"],
                              component["w"], component["h"]);
        }
        else if (type == "pngfile")
        {
            renderer.draw_png(component["src"],
                              component["x"], component["y"],
                              component["w"], component["h"]);
        }
    }
}

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

    // 读取文档基本参数
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
    Renderer::unitType unit = Renderer::PX; // 默认px
    {
        auto unitJson = json.find("unit");
        if (unitJson != json.end())
        { // Has "unit"
            string unitStr = *unitJson;
            if (unitStr == "px" || unitStr == "pt")
                unit = Renderer::PX;
            else if (unitStr == "in" || unitStr == "inch")
                unit = Renderer::IN;
            else if (unitStr == "mm")
                unit = Renderer::MM;
            else if (unitStr == "cm")
                unit = Renderer::CM;
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

    // 准备渲染器
    Renderer renderer(args.output, args.type, width, height, unit, ppi);

    // 渲染body
    auto body = json.find("body");
    if (body != json.end())
    { // Has "body"
        if (body->is_array())
            render(renderer, *body);
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
