#include "default.h"

#include "ComponentImage.h"
#include "ComponentImageMask.h"

namespace Render
{

ComponentImageMask::ComponentImageMask(const string &imageFilePath,
                                       double x, double y,
                                       double w, double h,
                                       cairo_surface_t *childSurface)
{
    type = ComponentTypeImageMask;

    cairo_t *cr = cairo_create(surface);

    ComponentImage mask(imageFilePath,
                        x, y,
                        w, h);

    cairo_set_source_surface(cr, childSurface, 0.0, 0.0);
    cairo_mask_surface(cr, mask.getSurface(), 0, 0);
    cairo_fill(cr);

    cairo_destroy(cr);
}

ComponentImageMask::ComponentImageMask(const Mat &image,
                                       double x, double y,
                                       double w, double h,
                                       cairo_surface_t *childSurface)
{
    type = ComponentTypeImageMask;

    cairo_t *cr = cairo_create(surface);

    ComponentImage mask(image,
                        x, y,
                        w, h);

    cairo_set_source_surface(cr, childSurface, 0.0, 0.0);
    cairo_mask_surface(cr, mask.getSurface(), 0, 0);
    cairo_fill(cr);

    cairo_destroy(cr);
}

ComponentImageMask::~ComponentImageMask()
{
}

} // namespace Render
