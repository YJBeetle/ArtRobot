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

#pragma once

#include <ArtRobot/Features.hpp>

namespace ArtRobot {

    enum class OutputType {
        Svg,
        Pdf,
        Pixmap,
        Png,
#ifdef WEBP_FOUND
        Webp,
#endif
#ifdef JPEG_FOUND
        Jpeg,
#endif
    };
}
