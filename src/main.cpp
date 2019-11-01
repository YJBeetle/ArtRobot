#include "default.h"

#include "Args.h"
#include "Color.h"
#include "Renderer.h"

enum ComponentType
{
    ComponentTypeUnknow = 0,
    ComponentTypeRectangle,
    ComponentTypeImage,
    ComponentTypeImageMask,
    ComponentTypeText,
    ComponentTypeRepeat,
    ComponentTypeGroup,
};

void renderComponent(Renderer &renderer, Json &component)
{
    ComponentType componentType = ComponentTypeUnknow;
    {
        auto &typeJson = component["type"];
        if (typeJson.is_string())
        {
            if (typeJson == "rectangle")
                componentType = ComponentTypeRectangle;
            else if (typeJson == "image")
                componentType = ComponentTypeImage;
            else if (typeJson == "imageMask")
                componentType = ComponentTypeImageMask;
            else if (typeJson == "text")
                componentType = ComponentTypeText;
            else if (typeJson == "repeat")
                componentType = ComponentTypeRepeat;
            else if (typeJson == "group")
                componentType = ComponentTypeGroup;
        }
    }

    switch (componentType)
    {
    case ComponentTypeRectangle:
    {
        auto &colorJ = component["color"];
        auto &xJ = component["x"];
        auto &yJ = component["y"];
        auto &wJ = component["w"];
        auto &hJ = component["h"];

        string color = colorJ.is_string() ? (string)colorJ : "000000";
        double x = xJ.is_number() ? (double)xJ : 0;
        double y = yJ.is_number() ? (double)yJ : 0;
        double w = wJ.is_number() ? (double)wJ : 100;
        double h = hJ.is_number() ? (double)hJ : 100;

        renderer.draw_rectangle(color.c_str(),
                                x, y,
                                w, h);
    }
    break;
    case ComponentTypeImage:
    {
        auto &srcJ = component["src"];
        auto &xJ = component["x"];
        auto &yJ = component["y"];
        auto &wJ = component["w"];
        auto &hJ = component["h"];

        string src = srcJ.is_string() ? (string)srcJ : "";
        double x = xJ.is_number() ? (double)xJ : 0;
        double y = yJ.is_number() ? (double)yJ : 0;
        double w = wJ.is_number() ? (double)wJ : 100;
        double h = hJ.is_number() ? (double)hJ : 100;

        renderer.draw_svg(src,
                          x, y,
                          w, h);

        // renderer.draw_png
    }
    break;
    case ComponentTypeImageMask:
    {
    }
    break;
    case ComponentTypeText:
    {
        auto &contentJ = component["content"];
        auto &colorJ = component["color"];
        auto &xJ = component["x"];
        auto &yJ = component["y"];
        auto &wJ = component["w"];
        auto &hJ = component["h"];
        auto &writingModeJ = component["writingMode"];
        auto &wordWrapJ = component["wordWrap"];
        auto &horizontalAlignJ = component["horizontalAlign"];
        auto &verticalAlignJ = component["verticalAlign"];
        auto &fontFamilyJ = component["fontFamily"];
        auto &fontSizeJ = component["fontSize"];
        auto &lineSpacingJ = component["lineSpacing"];
        auto &wordSpacingJ = component["wordSpacing"];

        string content = contentJ.is_string() ? (string)contentJ : "";
        string color = colorJ.is_string() ? (string)colorJ : "000000";
        double x = xJ.is_number() ? (double)xJ : 0;
        double y = yJ.is_number() ? (double)yJ : 0;
        double w = wJ.is_number() ? (double)wJ : 100;
        double h = hJ.is_number() ? (double)hJ : 100;
        int writingMode = writingModeJ.is_number() ? (int)writingModeJ : 0; // 书写方向，同css中writing-mode，0=horizontal-tb，1=vertical-rl，2=vertical-lr
        bool wordWrap = wordWrapJ.is_boolean() ? (bool)wordWrapJ : true;
        int horizontalAlign = horizontalAlignJ.is_number() ? (int)horizontalAlignJ : 0; // 水平对齐方式，-1为左对齐，0居中，1右对齐
        int verticalAlign = verticalAlignJ.is_number() ? (int)verticalAlignJ : 0;       // 垂直对齐方式，-1为顶部对齐，0居中，1底部对齐
        string fontFamily = fontFamilyJ.is_string() ? (string)fontFamilyJ : "";
        double fontSize = fontSizeJ.is_number() ? (double)fontSizeJ : 14;
        double lineSpacing = lineSpacingJ.is_number() ? (double)lineSpacingJ : 1;
        double wordSpacing = wordSpacingJ.is_number() ? (double)wordSpacingJ : 0;

        renderer.draw_text(content,
                           "Lantinghei.ttc",
                           0,
                           fontSize,
                           horizontalAlign,
                           color.c_str(),
                           x, y);
    }
    break;
    case ComponentTypeRepeat:
    {
    }
    break;
    case ComponentTypeGroup:
    {
    }
    break;
    default:
        break;
    }
}

void renderComponents(Renderer &renderer, Json &components)
{
    if (components.is_array())
    {
        for (auto &component : components) // 循环处理该成员中的元素
        {
            renderComponent(renderer, component);
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
    auto &wJ = json["w"];
    auto &hJ = json["h"];
    double w = wJ.is_number() ? (double)wJ : 200;
    double h = hJ.is_number() ? (double)hJ : 200;
    Renderer::unitType unit = Renderer::PX; // 默认px
    {
        auto &unitJ = json["unit"];
        if (unitJ.is_string())
        { // Has "unit"
            string unitStr = unitJ;
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
    auto &ppiJ = json["ppi"];
    double ppi = ppiJ.is_number() ? (double)ppiJ : 72;

    // 准备渲染器
    Renderer renderer(args.output, args.type, w, h, unit, ppi);

    // 渲染body
    auto body = json.find("body");
    if (body != json.end())
    { // Has "body"
        renderComponents(renderer, *body);
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
