#include "CommonIncludes.h"

#include "ArtRobot/ArtRobot.h"
#include "Args.h"

using namespace ArtRobot;

shared_ptr<Component::Base> renderComponent(Json &componentJson)
{
    Component::Type componentType = Component::TypeUnknow;
    {
        auto &typeJson = componentJson["type"];
        if (typeJson.is_string())
        {
            if (typeJson == "rectangle")
                componentType = Component::TypeRectangle;
            else if (typeJson == "svg")
                componentType = Component::TypeSvg;
            else if (typeJson == "image")
                componentType = Component::TypeImage;
            else if (typeJson == "imageMask")
                componentType = Component::TypeImageMask;
            else if (typeJson == "text")
                componentType = Component::TypeText;
            else if (typeJson == "repeat")
                componentType = Component::TypeRepeat;
            else if (typeJson == "group")
                componentType = Component::TypeGroup;
        }
    }

    auto &nameJ = componentJson["name"];
    auto &xJ = componentJson["x"];
    auto &yJ = componentJson["y"];
    auto &wJ = componentJson["w"];
    auto &hJ = componentJson["h"];
    auto &rJ = componentJson["r"];
    std::string name = nameJ.is_string() ? (std::string)nameJ : "";
    double x = xJ.is_number() ? (double)xJ : 0;
    double y = yJ.is_number() ? (double)yJ : 0;
    double w = wJ.is_number() ? (double)wJ : 100;
    double h = hJ.is_number() ? (double)hJ : 100;
    double r = rJ.is_number() ? (double)rJ : 0;

    switch (componentType)
    {
    case Component::TypeRectangle:
    {
        auto &colorJ = componentJson["color"];
        string color = colorJ.is_string() ? (string)colorJ : "000000";

        return make_shared<Component::Rectangle>(name, x, y, w, h, r,
                                                 color.c_str());
    }
    case Component::TypeSvg:
    {
        auto &srcJ = componentJson["src"];
        string src = srcJ.is_string() ? (string)srcJ : "";

        return make_shared<Component::Svg>(name, x, y, w, h, r,
                                           src);
    }
    case Component::TypeImage:
    {
        auto &srcJ = componentJson["src"];
        string src = srcJ.is_string() ? (string)srcJ : "";

        return make_shared<Component::Image>(name, x, y, w, h, r,
                                             src);
    }
    case Component::TypeImageMask:
    {
        auto &srcJ = componentJson["src"];
        auto &childJ = componentJson["child"];
        string src = srcJ.is_string() ? (string)srcJ : "";
        auto child = renderComponent(childJ);

        return make_shared<Component::ImageMask>(name, x, y, w, h, r,
                                                 src, child);
    }
    case Component::TypeText:
    {
        auto &contentJ = componentJson["content"];
        auto &colorJ = componentJson["color"];
        auto &writingModeJ = componentJson["writingMode"];
        auto &wordWrapJ = componentJson["wordWrap"];
        auto &horizontalAlignJ = componentJson["horizontalAlign"];
        auto &verticalAlignJ = componentJson["verticalAlign"];
        auto &fontFamilyJ = componentJson["fontFamily"];
        auto &fontSizeJ = componentJson["fontSize"];
        auto &lineSpacingJ = componentJson["lineSpacing"];
        auto &wordSpacingJ = componentJson["wordSpacing"];

        string content = contentJ.is_string() ? (string)contentJ : "";
        string color = colorJ.is_string() ? (string)colorJ : "000000";
        int writingMode = writingModeJ.is_number() ? (int)writingModeJ : 0; // 书写方向，同css中writing-mode，0=horizontal-tb，1=vertical-rl，2=vertical-lr
        bool wordWrap = wordWrapJ.is_boolean() ? (bool)wordWrapJ : true;
        int horizontalAlign = horizontalAlignJ.is_number() ? (int)horizontalAlignJ : 0; // 水平对齐方式，-1为左对齐，0居中，1右对齐
        int verticalAlign = verticalAlignJ.is_number() ? (int)verticalAlignJ : 0;       // 垂直对齐方式，-1为顶部对齐，0居中，1底部对齐
        string fontFamily = fontFamilyJ.is_string() ? (string)fontFamilyJ : "";
        double fontSize = fontSizeJ.is_number() ? (double)fontSizeJ : 14;
        double lineSpacing = lineSpacingJ.is_number() ? (double)lineSpacingJ : 1;
        double wordSpacing = wordSpacingJ.is_number() ? (double)wordSpacingJ : 0;

        return make_shared<Component::Text>(name, x, y, w, h, r,
                                            content,
                                            "Lantinghei.ttc",
                                            0,
                                            fontSize,
                                            horizontalAlign,
                                            color.c_str());
    }
    case Component::TypeRepeat:
    {
        return make_shared<Component::Repeat>(name, x, y, w, h, r);
    }
    case Component::TypeGroup:
    {
        auto &childJson = componentJson["child"];
        if(childJson.is_array())
        {
            auto componentGroup = make_shared<Component::Group>(name);
            for (auto &componentJson : childJson) // 循环处理该成员中的元素
            {
                auto component = renderComponent(componentJson);
                componentGroup->addChild(component);
            }
            return componentGroup;
        }
        else
            return make_shared<Component::Base>();
    }
    default:
        return make_shared<Component::Base>();
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

    // 绘制body
    auto &bodyJson = json["body"];
    auto body = renderComponent(bodyJson);

    // 渲染
    Renderer renderer(args.type, w, h, unit, ppi);
    renderer.render(body->getSurface());

    // 保存
    renderer.saveToFile(args.output);

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
