#pragma once

#include "default.h"

#include "Component.h"

namespace Render
{

class ComponentImage : public Component
{
private:
    enum ImageFileType
    {
        PNG,
        SVG,
        JPG,
    };

public:
    ComponentImage(const string &imageFilePath,
                                   double x, double y,
                                   double width, double height);
    ~ComponentImage();
};

} // namespace Render
