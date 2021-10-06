/*
 * Copyright 2020 YJBeetle
 *
 * Authors:
 *  YJBeetle <YJBeetle@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 3.  See
 * the LICENSE file in the top-level directory.
 *
 */

#include "./Base.hpp"

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
      _x(__x),
      _y(__y),
      _w(__w),
      _h(__h),
      _r(__r)
{
    if (type != TypeUnknow)
    {
        surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
        cr = cairo_create(surface);
        cairo_translate(cr, _w / 2, _h / 2);
        cairo_rotate(cr, _r * M_PI / 180);
        cairo_translate(cr, -_w / 2, -_h / 2);
    }
}

Base::~Base()
{
    if(cr)
        cairo_destroy(cr);
    if(surface)
        cairo_surface_destroy(surface);
}

void Base::finish()
{
    if(cr)
        cairo_destroy(cr);
    cr = nullptr;
    // cairo_surface_finish(surface);
}

cairo_surface_t *Base::getSurface()
{
    return (type != TypeUnknow)
               ? surface
               : nullptr;
}

} // namespace Component
} // namespace ArtRobot
