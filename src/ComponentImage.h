#pragma once

#include "default.h"

#include "Component.h"

namespace Render
{

void drawImageSurface(cairo_t *cr,
                      double w, double h,
                      cairo_surface_t *imageSurface,
                      int imageW, int imageH);

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
    ComponentImage(double x, double y,
                   double w, double h,
                   double r,
                   const string &imageFilePath);
    ComponentImage(double x, double y,
                   double w, double h,
                   double r,
                   const Mat &imageMatRead);
    ~ComponentImage();
};

} // namespace Render
