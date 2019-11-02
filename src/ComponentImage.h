#pragma once

#include "default.h"

#include "Component.h"

namespace Render
{

void drawImageSurface(cairo_t *cr, cairo_surface_t *imageSurface,
                      int imageW, int imageH,
                      double x, double y,
                      double w, double h);

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
                   double w, double h);
    ComponentImage(const Mat &imageMatRead,
                   double x, double y,
                   double w, double h);
    ~ComponentImage();
};

} // namespace Render
