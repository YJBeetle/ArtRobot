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
    ComponentText(const string &text,
                           const string &fontfile,
                           long face_index,
                           double font_size,
                           int8_t alignment,
                           Color argb,
                           double x, double y);
    ~ComponentText();
};

} // namespace Render
