#pragma once

#include <string>
#include <cairo-ft.h>

#include "ArtRobot/Types/Color.h"
#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Text : public Base
{
private:
public:
    Text(std::string __name,
         double __x, double __y,
         double __w, double __h,
         double __r,
         const std::string &text,
         const std::string &fontfile,
         long face_index,
         double font_size,
         int8_t alignment,
         Color argb);
    ~Text();
};

} // namespace Component
} // namespace ArtRobot