#pragma once

#include <string>
#include <cairo-ft.h>

using namespace std;

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
    Text(double x, double y,
         double w, double h,
         double r,
         const string &text,
         const string &fontfile,
         long face_index,
         double font_size,
         int8_t alignment,
         Color argb);
    ~Text();
};

} // namespace Component
} // namespace ArtRobot
