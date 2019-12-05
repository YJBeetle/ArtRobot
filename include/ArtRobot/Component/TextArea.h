#pragma once

#include <string>
#include <cairo-ft.h>
#include <pango/pangocairo.h>

#include "ArtRobot/Types/Color.h"
#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class TextArea : public Base
{
private:
public:
    TextArea(std::string __name,
             double __x, double __y,
             double __w, double __h,
             double __r,
             const std::string &content,    // 内容
             Color color,                   // 颜色
             const std::string &fontFamily, // 字体
             int fontWeight,                // 粗细
             double fontSize,               // 字号
             int8_t horizontalAlign,        // 水平对齐方式，0为左对齐，1居中，2右对齐
             int8_t verticalAlign);         // 垂直对齐方式，0为第一行基线对齐，1为顶部对齐，2垂直居中对齐，3底部对齐
    ~TextArea();
};

} // namespace Component
} // namespace ArtRobot
