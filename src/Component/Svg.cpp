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

#include <memory>
#include <stdexcept>
#include <string>

namespace ArtRobot {
    namespace Component {

        namespace {
            using RsvgHandlePtr = std::unique_ptr<RsvgHandle, decltype(&g_object_unref)>;

            [[noreturn]] void throwSvgError(const std::string &action, GError *error) {
                const std::string detail = error ? error->message : "unknown error";
                if (error)
                    g_error_free(error);
                throw std::runtime_error(action + ": " + detail);
            }
        }

        void drawSvg(cairo_t *cr,
                     double w, double h,
                     RsvgHandle *svg) {
            double intrinsicWidth = 0;
            double intrinsicHeight = 0;
            const bool hasIntrinsicSize = rsvg_handle_get_intrinsic_size_in_pixels(svg, &intrinsicWidth, &intrinsicHeight);
            if ((!hasIntrinsicSize || intrinsicWidth <= 0 || intrinsicHeight <= 0) && (w <= 0 || h <= 0))
                throw std::runtime_error("SVG has no usable intrinsic size");

            double targetWidth = w;
            double targetHeight = h;
            if (targetWidth <= 0)
                targetWidth = targetHeight * intrinsicWidth / intrinsicHeight;
            if (targetHeight <= 0)
                targetHeight = targetWidth * intrinsicHeight / intrinsicWidth;

            RsvgRectangle viewport{0, 0, targetWidth, targetHeight};
            GError *error = nullptr;
            if (!rsvg_handle_render_document(svg, cr, &viewport, &error)) {
                throwSvgError("Failed to render SVG", error);
            }
        }

        Svg::Svg(std::string name, Transform transform, double width, double height,
                const std::string &src)
                : Base({name, width, height}, transform) {
            GError *error = nullptr;
            RsvgHandlePtr svg(rsvg_handle_new_from_file(src.c_str(), &error), &g_object_unref);
            if (!svg)
                throwSvgError("Failed to load SVG file", error);
            drawSvg(cr, width, height, svg.get());
        }

        Svg::Svg(std::string name, Transform transform, double width, double height,
                 const unsigned char *data, size_t len)
                : Base({name, width, height}, transform) {
            GError *error = nullptr;
            RsvgHandlePtr svg(rsvg_handle_new_from_data(data, len, &error), &g_object_unref);
            if (!svg)
                throwSvgError("Failed to load SVG data", error);
            drawSvg(cr, width, height, svg.get());
        }

        Svg::~Svg() {
        }

    } // namespace Component
} // namespace ArtRobot

#endif
