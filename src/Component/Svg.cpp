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

#include "./Svg.hpp"

#ifdef RSVG_FOUND

namespace ArtRobot {
    namespace Component {

        void drawSvg(cairo_t *cr,
                     double w, double h,
                     RsvgHandle *svg) {
            if (w || h) {
                double scaleX, scaleY;
                RsvgDimensionData dimension_data;
                rsvg_handle_get_dimensions(svg, &dimension_data);
                scaleX = w / (double) dimension_data.width;
                scaleY = h / (double) dimension_data.height;
                cairo_scale(cr, scaleX, scaleY);
            }
            rsvg_handle_render_cairo(svg, cr);
        }

        Svg::Svg(std::string name, double width, double height, Transform transform,
                 const std::string &src)
                : Base({name, width, height}, transform) {
            FILE *imageFile = fopen(src.c_str(), "rb"); // 判断文件存在
            if (imageFile) {
                fclose(imageFile);

                RsvgHandle *svg = rsvg_handle_new_from_file(src.c_str(), nullptr); // TODO 错误处理

                drawSvg(cr,
                        width, height,
                        svg);

                rsvg_handle_close(svg, nullptr); // 释放handle
            }
        }

        Svg::Svg(std::string name, double width, double height, Transform transform,
                 unsigned char *data, size_t len)
                : Base({name, width, height}, transform) {
            RsvgHandle *svg = rsvg_handle_new_from_data(data, len, nullptr); // TODO 错误处理

            drawSvg(cr,
                    width, height,
                    svg);

            rsvg_handle_close(svg, nullptr);
        }

        Svg::~Svg() {
        }

    } // namespace Component
} // namespace ArtRobot

#endif
