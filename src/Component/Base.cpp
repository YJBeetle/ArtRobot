#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

Base::Base()
    : type(TypeUnknow)
{
}

Base::Base(Type __type, std::string __name)
    : type(__type),
      name(__name)
{
    if (type != TypeUnknow)
    {
        surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
        cr = cairo_create(surface);
    }
}

Base::Base(Type __type, std::string __name,
           double __x, double __y,
           double __w, double __h,
           double __r)
    : type(__type),
      name(__name),
      x(__x),
      y(__y),
      w(__w),
      h(__h),
      r(__r)
{
    if (type != TypeUnknow)
    {
        surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
        cr = cairo_create(surface);
        cairo_translate(cr, x, y);
        cairo_translate(cr, w / 2, h / 2);
        cairo_rotate(cr, r * M_PI / 180);
        cairo_translate(cr, -w / 2, -h / 2);
    }
}

Base::~Base()
{
    if (type != TypeUnknow)
    {
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }
}

cairo_surface_t *Base::getSurface()
{
    return (type != TypeUnknow)
               ? surface
               : nullptr;
}

} // namespace Component
} // namespace ArtRobot
