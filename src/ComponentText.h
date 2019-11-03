#pragma once

#include "default.h"

#include "Color.h"
#include "Component.h"

namespace Render
{

class ComponentText : public Component
{
private:
public:
    ComponentText(double x, double y,
                  double w, double h,
                  double r,
                  const string &text,
                  const string &fontfile,
                  long face_index,
                  double font_size,
                  int8_t alignment,
                  Color argb);
    ~ComponentText();
};

} // namespace Render
