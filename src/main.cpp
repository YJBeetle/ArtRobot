#include "CommonIncludes.h"

#include <ArtRobot/ArtRobot.hpp>
#include "./Args.hpp"

using namespace ArtRobot;

inline bool lowercaseEq(string str1, string str2) {
    transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
    return str1 == str2;
}

shared_ptr<Component::Base> renderComponent(Json &componentJson, int depth) {
    auto &nameJ = componentJson["name"];
    auto &xJ = componentJson["x"];
    auto &yJ = componentJson["y"];
    auto &wJ = componentJson["w"];
    auto &hJ = componentJson["h"];
    auto &rJ = componentJson["r"];
    std::string name = nameJ.is_string() ? (std::string) nameJ : "";
    double x = xJ.is_number() ? (double) xJ : 0;
    double y = yJ.is_number() ? (double) yJ : 0;
    double w = wJ.is_number() ? (double) wJ : 100;
    double h = hJ.is_number() ? (double) hJ : 100;
    double r = rJ.is_number() ? (double) rJ : 0;

    auto &typeJson = componentJson["type"];
    if (typeJson.is_string()) {
        if (lowercaseEq(typeJson, "rectangle")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "rectangle" << "-" << name << std::endl;
            auto &colorJ = componentJson["color"];
            string color = colorJ.is_string() ? (string) colorJ : "000000";
            return make_shared<Component::Rectangle>(name, Transform{.x=x, .y=y, .rotate=r}, w, h, color.c_str());
        } else if (lowercaseEq(typeJson, "rectangleRound")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "rectangleRound" << "-" << name << std::endl;
            auto &colorJ = componentJson["color"];
            auto &angleJ = componentJson["angle"];
            auto &angleTLJ = componentJson["angleTL"];
            auto &angleTRJ = componentJson["angleTR"];
            auto &angleBLJ = componentJson["angleBR"];
            auto &angleBRJ = componentJson["angleBL"];
            string color = colorJ.is_string() ? (string) colorJ : "000000";
            double angleTL = angleTLJ.is_number() ? (double) angleTLJ : angleJ.is_number() ? (double) angleJ : 10;
            double angleTR = angleTRJ.is_number() ? (double) angleTRJ : angleJ.is_number() ? (double) angleJ : 10;
            double angleBR = angleBLJ.is_number() ? (double) angleBLJ : angleJ.is_number() ? (double) angleJ : 10;
            double angleBL = angleBRJ.is_number() ? (double) angleBRJ : angleJ.is_number() ? (double) angleJ : 10;
            return make_shared<Component::RectangleRound>(name, Transform{.x=x, .y=y, .rotate=r}, w, h,
                                                          angleTL, angleTR, angleBR, angleBL, color.c_str());
        } else if (lowercaseEq(typeJson, "circle")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "circle" << "-" << name << std::endl;
            auto &colorJ = componentJson["color"];
            string color = colorJ.is_string() ? (string) colorJ : "000000";
            return make_shared<Component::Circle>(name, Transform{.x=x, .y=y, .rotate=r}, w, h, color.c_str());
        } else if (lowercaseEq(typeJson, "svg")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "svg" << "-" << name << std::endl;
            auto &srcJ = componentJson["src"];
            string src = srcJ.is_string() ? (string) srcJ : "";
            return make_shared<Component::Svg>(name, w, h, Transform{.x=x, .y=y, .rotate=r}, src);
        } else if (lowercaseEq(typeJson, "image")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "image" << "-" << name << std::endl;
            auto &srcJ = componentJson["src"];
            string src = srcJ.is_string() ? (string) srcJ : "";
            return make_shared<Component::Image>(name, Transform{.x=x, .y=y, .rotate=r}, src, w, h);
        } else if (lowercaseEq(typeJson, "imageMask")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "imageMask" << "-" << name << std::endl;
            auto &maskJ = componentJson["mask"];
            auto &childJ = componentJson["child"];
            auto mask = renderComponent(maskJ, depth + 1);
            auto child = renderComponent(childJ, depth + 1);
            return make_shared<Component::Mask>(name, w, h, Transform{.x=x, .y=y, .rotate=r}, mask, child);
        } else if (lowercaseEq(typeJson, "text")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "text" << "-" << name << std::endl;
            auto &contentJ = componentJson["content"];
            auto &colorJ = componentJson["color"];
            auto &fontFamilyJ = componentJson["fontFamily"];
            auto &fontWeightJ = componentJson["fontWeight"];
            auto &fontSizeJ = componentJson["fontSize"];
            auto &horizontalAlignJ = componentJson["horizontalAlign"];
            auto &verticalAlignJ = componentJson["verticalAlign"];
            auto &maxWidthJ = componentJson["maxWidth"];
            auto &lineSpacingJ = componentJson["lineSpacing"];
            auto &wordSpacingJ = componentJson["wordSpacing"];
            auto &writingModeJ = componentJson["writingMode"]; // 待议
            auto &wordWrapJ = componentJson["wordWrap"];       // 待议
            string content = contentJ.is_string() ? (string) contentJ : "";
            string color = colorJ.is_string() ? (string) colorJ : "000000";
            string fontFamily = fontFamilyJ.is_string() ? (string) fontFamilyJ : "";
            int fontWeight = fontFamilyJ.is_number() ? (int) fontWeightJ : 400;
            double fontSize = fontSizeJ.is_number() ? (double) fontSizeJ : 14;
            int horizontalAlign = horizontalAlignJ.is_number() ? (int) horizontalAlignJ : 0;
            int verticalAlign = verticalAlignJ.is_number() ? (int) verticalAlignJ : 0;
            int maxWidth = maxWidthJ.is_number() ? (int) maxWidthJ : 0;
            double lineSpacing = lineSpacingJ.is_number() ? (double) lineSpacingJ : 0;
            double wordSpacing = wordSpacingJ.is_number() ? (double) wordSpacingJ : 0;
            int writingMode = writingModeJ.is_number() ? (int) writingModeJ : 0;
            bool wordWrap = wordWrapJ.is_boolean() ? (bool) wordWrapJ : true;
            return make_shared<Component::Text>(name, Transform{.x=x, .y=y, .rotate=r},
                                                content, color.c_str(),
                                                fontFamily,
                                                fontWeight,
                                                fontSize,
                                                HorizontalAlign(horizontalAlign),
                                                VerticalAlign(verticalAlign),
                                                maxWidth,
                                                lineSpacing,
                                                wordSpacing);
        } else if (lowercaseEq(typeJson, "textArea")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "textArea" << "-" << name << std::endl;
            auto &contentJ = componentJson["content"];
            auto &colorJ = componentJson["color"];
            auto &fontFamilyJ = componentJson["fontFamily"];
            auto &fontWeightJ = componentJson["fontWeight"];
            auto &fontSizeJ = componentJson["fontSize"];
            auto &horizontalAlignJ = componentJson["horizontalAlign"];
            auto &verticalAlignJ = componentJson["verticalAlign"];
            auto &lineSpacingJ = componentJson["lineSpacing"];
            auto &wordSpacingJ = componentJson["wordSpacing"];
            auto &writingModeJ = componentJson["writingMode"]; // 待议
            auto &wordWrapJ = componentJson["wordWrap"];       // 待议
            string content = contentJ.is_string() ? (string) contentJ : "";
            string color = colorJ.is_string() ? (string) colorJ : "000000";
            string fontFamily = fontFamilyJ.is_string() ? (string) fontFamilyJ : "";
            int fontWeight = fontFamilyJ.is_number() ? (int) fontWeightJ : 400;
            double fontSize = fontSizeJ.is_number() ? (double) fontSizeJ : 14;
            int horizontalAlign = horizontalAlignJ.is_number() ? (int) horizontalAlignJ : 0;
            int verticalAlign = verticalAlignJ.is_number() ? (int) verticalAlignJ : 0;
            double lineSpacing = lineSpacingJ.is_number() ? (double) lineSpacingJ : 0;
            double wordSpacing = wordSpacingJ.is_number() ? (double) wordSpacingJ : 0;
            int writingMode = writingModeJ.is_number() ? (int) writingModeJ : 0;
            bool wordWrap = wordWrapJ.is_boolean() ? (bool) wordWrapJ : true;
            return make_shared<Component::TextArea>(name, Transform{.x=x, .y=y, .rotate=r}, w, h,
                                                    content, color.c_str(),
                                                    fontFamily,
                                                    fontWeight,
                                                    fontSize,
                                                    HorizontalAlign(horizontalAlign),
                                                    VerticalAlign(verticalAlign),
                                                    lineSpacing,
                                                    wordSpacing);
        } else if (lowercaseEq(typeJson, "repeat")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "repeat" << "-" << name << std::endl;
            return make_shared<Component::Repeat>(name, w, h, Transform{.x=x, .y=y, .rotate=r});
        } else if (lowercaseEq(typeJson, "group")) {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "group" << "-" << name << std::endl;
            auto &childJson = componentJson["child"];
            if (childJson.is_array()) {
                auto componentGroup = make_shared<Component::Group>(name, Transform{.x=x, .y=y, .rotate=r});
                for (auto &componentJson: childJson) // 循环处理该成员中的元素
                {
                    auto component = renderComponent(componentJson, depth + 1);
                    componentGroup->addChild(component);
                }
                return componentGroup;
            } else
                return make_shared<Component::Base>();
        } else {
            for (int i = 0; i < depth; i++) std::cout << "\t";
            std::cout << "unknow" << "-" << name << std::endl;
            return make_shared<Component::Base>();
        }
    } else {
        for (int i = 0; i < depth; i++) std::cout << "\t";
        std::cout << "invalid" << "-" << name << std::endl;
        return make_shared<Component::Base>();
    }
}

int main(int argc, char *argv[]) {
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
    double w = wJ.is_number() ? (double) wJ : 200;
    double h = hJ.is_number() ? (double) hJ : 200;
    Unit unit = Unit::Pixel; // 默认px
    {
        auto &unitJ = json["unit"];
        if (unitJ.is_string()) { // Has "unit"
            string unitStr = unitJ;
            if (unitStr == "px" || unitStr == "pt")
                unit = Unit::Pixel;
            else if (unitStr == "in" || unitStr == "inch")
                unit = Unit::Inch;
            else if (unitStr == "mm")
                unit = Unit::Millimeter;
            else if (unitStr == "cm")
                unit = Unit::Centimeter;
        }
    }
    auto &ppiJ = json["ppi"];
    double ppi = ppiJ.is_number() ? (double) ppiJ : 72;

    // 绘制body
    auto &bodyJson = json["body"];
    auto body = renderComponent(bodyJson, 0);

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
