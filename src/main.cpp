#include "default.h"

#include "Args.h"
#include "Color.h"
#include "Renderer.h"
#include "Component.h"
#include "ComponentGroup.h"
#include "ComponentImage.h"
#include "ComponentImageMask.h"
#include "ComponentRectangle.h"
#include "ComponentRepeat.h"
#include "ComponentText.h"

using namespace Render;

shared_ptr<Component> renderComponents(Json &componentsJson);

shared_ptr<Component> renderComponent(Json &componentJson)
{
    ComponentType componentType = ComponentTypeUnknow;
    {
        auto &typeJson = componentJson["type"];
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
        auto &colorJ = componentJson["color"];
        auto &xJ = componentJson["x"];
        auto &yJ = componentJson["y"];
        auto &wJ = componentJson["w"];
        auto &hJ = componentJson["h"];

        string color = colorJ.is_string() ? (string)colorJ : "000000";
        double x = xJ.is_number() ? (double)xJ : 0;
        double y = yJ.is_number() ? (double)yJ : 0;
        double w = wJ.is_number() ? (double)wJ : 100;
        double h = hJ.is_number() ? (double)hJ : 100;

        return make_shared<ComponentRectangle>(color.c_str(),
                                               x, y,
                                               w, h);
    }
    case ComponentTypeImage:
    {
        auto &srcJ = componentJson["src"];
        auto &xJ = componentJson["x"];
        auto &yJ = componentJson["y"];
        auto &wJ = componentJson["w"];
        auto &hJ = componentJson["h"];

        string src = srcJ.is_string() ? (string)srcJ : "";
        double x = xJ.is_number() ? (double)xJ : 0;
        double y = yJ.is_number() ? (double)yJ : 0;
        double w = wJ.is_number() ? (double)wJ : 100;
        double h = hJ.is_number() ? (double)hJ : 100;

        return make_shared<ComponentImage>(src,
                                           x, y,
                                           w, h);
    }
    case ComponentTypeImageMask:
    {
        return make_shared<Component>();
    }
    case ComponentTypeText:
    {
        auto &contentJ = componentJson["content"];
        auto &colorJ = componentJson["color"];
        auto &xJ = componentJson["x"];
        auto &yJ = componentJson["y"];
        auto &wJ = componentJson["w"];
        auto &hJ = componentJson["h"];
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

        return make_shared<ComponentText>(content,
                                          "Lantinghei.ttc",
                                          0,
                                          fontSize,
                                          horizontalAlign,
                                          color.c_str(),
                                          x, y);
    }
    case ComponentTypeRepeat:
    {
        return make_shared<Component>();
    }
    case ComponentTypeGroup:
    {
        auto &childJson = componentJson["child"];
        return renderComponents(childJson);
    }
    default:
    {
        return make_shared<Component>();
    }
    }
}

shared_ptr<Component> renderComponents(Json &componentsJson)
{
    auto componentGroup = make_shared<ComponentGroup>();
    if (componentsJson.is_array())
        for (auto &componentJson : componentsJson) // 循环处理该成员中的元素
        {
            auto component = renderComponent(componentJson);
            componentGroup->addChild(component->getSurface());
        }
    return componentGroup;
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
    Renderer renderer(w, h, unit, ppi);

    // 渲染body
    auto &bodyJson = json["body"];
    auto mainComponent = renderComponents(bodyJson);
    renderer.render(mainComponent->getSurface());

    // 保存
    renderer.save(args.output, args.type);

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
