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

#include "./Color.hpp"

#include <cstring>
#include <cctype>

namespace ArtRobot {

    Color::Color() : value(0x000000FF) {
    }

    Color::Color(uint32_t colorVal) {
        value = colorVal;
    }

    Color::Color(const char *colorStr) {
        if (colorStr[0] == '#') {
            colorStr++;
            uint32_t intColor;
            if (strlen(colorStr) <= 6)
                intColor = 0x000000FF; // 黑不透明
            else
                intColor = 0x00000000; // 黑透明
            for (int i = 7; i >= 0 && *colorStr; i--) {
                if (*colorStr == 0)
                    break;
                intColor |= ((*colorStr >= '0' && *colorStr <= '9')
                             ? *colorStr - '0'
                             : (*colorStr >= 'A' && *colorStr <= 'F')
                               ? *colorStr - 'A' + 0xA
                               : (*colorStr >= 'a' && *colorStr <= 'f')
                                 ? *colorStr - 'a' + 0xA
                                 : 0)
                        << (4 * i);
                colorStr++;
            }
            value = intColor;
        } else {
            char colorStrLower[strlen(colorStr) + 1];
            for (int i = 0; i < sizeof(colorStrLower); ++i)
                colorStrLower[i] = tolower(colorStr[i]);

            if (strcmp(colorStrLower, "black") == 0) value = 0x000000ff;
            else if (strcmp(colorStrLower, "silver") == 0) value = 0xc0c0c0ff;
            else if (strcmp(colorStrLower, "gray") == 0) value = 0x808080ff;
            else if (strcmp(colorStrLower, "white") == 0) value = 0xffffffff;
            else if (strcmp(colorStrLower, "maroon") == 0) value = 0x800000ff;
            else if (strcmp(colorStrLower, "red") == 0) value = 0xff0000ff;
            else if (strcmp(colorStrLower, "purple") == 0) value = 0x800080ff;
            else if (strcmp(colorStrLower, "fuchsia") == 0) value = 0xff00ffff;
            else if (strcmp(colorStrLower, "green") == 0) value = 0x008000ff;
            else if (strcmp(colorStrLower, "lime") == 0) value = 0x00ff00ff;
            else if (strcmp(colorStrLower, "olive") == 0) value = 0x808000ff;
            else if (strcmp(colorStrLower, "yellow") == 0) value = 0xffff00ff;
            else if (strcmp(colorStrLower, "navy") == 0) value = 0x000080ff;
            else if (strcmp(colorStrLower, "blue") == 0) value = 0x0000ffff;
            else if (strcmp(colorStrLower, "teal") == 0) value = 0x008080ff;
            else if (strcmp(colorStrLower, "aqua") == 0) value = 0x00ffffff;

            else if (strcmp(colorStrLower, "aliceblue") == 0) value = 0xf0f8ffff;
            else if (strcmp(colorStrLower, "antiquewhite") == 0) value = 0xfaebd7ff;
            else if (strcmp(colorStrLower, "aquamarine") == 0) value = 0x7fffd4ff;
            else if (strcmp(colorStrLower, "azure") == 0) value = 0xf0ffffff;
            else if (strcmp(colorStrLower, "beige") == 0) value = 0xf5f5dcff;
            else if (strcmp(colorStrLower, "bisque") == 0) value = 0xffe4c4ff;
            else if (strcmp(colorStrLower, "blanchedalmond") == 0) value = 0xffebcdff;
            else if (strcmp(colorStrLower, "blueviolet") == 0) value = 0x8a2be2ff;
            else if (strcmp(colorStrLower, "brown") == 0) value = 0xa52a2aff;
            else if (strcmp(colorStrLower, "burlywood") == 0) value = 0xdeb887ff;
            else if (strcmp(colorStrLower, "cadetblue") == 0) value = 0x5f9ea0ff;
            else if (strcmp(colorStrLower, "chartreuse") == 0) value = 0x7fff00ff;
            else if (strcmp(colorStrLower, "chocolate") == 0) value = 0xd2691eff;
            else if (strcmp(colorStrLower, "coral") == 0) value = 0xff7f50ff;
            else if (strcmp(colorStrLower, "cornflowerblue") == 0) value = 0x6495edff;
            else if (strcmp(colorStrLower, "cornsilk") == 0) value = 0xfff8dcff;
            else if (strcmp(colorStrLower, "crimson") == 0) value = 0xdc143cff;
            else if (strcmp(colorStrLower, "cyan") == 0) value = 0x00ffffff;
            else if (strcmp(colorStrLower, "darkblue") == 0) value = 0x00008bff;
            else if (strcmp(colorStrLower, "darkcyan") == 0) value = 0x008b8bff;
            else if (strcmp(colorStrLower, "darkgoldenrod") == 0) value = 0xb8860bff;
            else if (strcmp(colorStrLower, "darkgray") == 0) value = 0xa9a9a9ff;
            else if (strcmp(colorStrLower, "darkgreen") == 0) value = 0x006400ff;
            else if (strcmp(colorStrLower, "darkgrey") == 0) value = 0xa9a9a9ff;
            else if (strcmp(colorStrLower, "darkkhaki") == 0) value = 0xbdb76bff;
            else if (strcmp(colorStrLower, "darkmagenta") == 0) value = 0x8b008bff;
            else if (strcmp(colorStrLower, "darkolivegreen") == 0) value = 0x556b2fff;
            else if (strcmp(colorStrLower, "darkorange") == 0) value = 0xff8c00ff;
            else if (strcmp(colorStrLower, "darkorchid") == 0) value = 0x9932ccff;
            else if (strcmp(colorStrLower, "darkred") == 0) value = 0x8b0000ff;
            else if (strcmp(colorStrLower, "darksalmon") == 0) value = 0xe9967aff;
            else if (strcmp(colorStrLower, "darkseagreen") == 0) value = 0x8fbc8fff;
            else if (strcmp(colorStrLower, "darkslateblue") == 0) value = 0x483d8bff;
            else if (strcmp(colorStrLower, "darkslategray") == 0) value = 0x2f4f4fff;
            else if (strcmp(colorStrLower, "darkslategrey") == 0) value = 0x2f4f4fff;
            else if (strcmp(colorStrLower, "darkturquoise") == 0) value = 0x00ced1ff;
            else if (strcmp(colorStrLower, "darkviolet") == 0) value = 0x9400d3ff;
            else if (strcmp(colorStrLower, "deeppink") == 0) value = 0xff1493ff;
            else if (strcmp(colorStrLower, "deepskyblue") == 0) value = 0x00bfffff;
            else if (strcmp(colorStrLower, "dimgray") == 0) value = 0x696969ff;
            else if (strcmp(colorStrLower, "dimgrey") == 0) value = 0x696969ff;
            else if (strcmp(colorStrLower, "dodgerblue") == 0) value = 0x1e90ffff;
            else if (strcmp(colorStrLower, "firebrick") == 0) value = 0xb22222ff;
            else if (strcmp(colorStrLower, "floralwhite") == 0) value = 0xfffaf0ff;
            else if (strcmp(colorStrLower, "forestgreen") == 0) value = 0x228b22ff;
            else if (strcmp(colorStrLower, "gainsboro") == 0) value = 0xdcdcdcff;
            else if (strcmp(colorStrLower, "ghostwhite") == 0) value = 0xf8f8ffff;
            else if (strcmp(colorStrLower, "gold") == 0) value = 0xffd700ff;
            else if (strcmp(colorStrLower, "goldenrod") == 0) value = 0xdaa520ff;
            else if (strcmp(colorStrLower, "greenyellow") == 0) value = 0xadff2fff;
            else if (strcmp(colorStrLower, "grey") == 0) value = 0x808080ff;
            else if (strcmp(colorStrLower, "honeydew") == 0) value = 0xf0fff0ff;
            else if (strcmp(colorStrLower, "hotpink") == 0) value = 0xff69b4ff;
            else if (strcmp(colorStrLower, "indianred") == 0) value = 0xcd5c5cff;
            else if (strcmp(colorStrLower, "indigo") == 0) value = 0x4b0082ff;
            else if (strcmp(colorStrLower, "ivory") == 0) value = 0xfffff0ff;
            else if (strcmp(colorStrLower, "khaki") == 0) value = 0xf0e68cff;
            else if (strcmp(colorStrLower, "lavender") == 0) value = 0xe6e6faff;
            else if (strcmp(colorStrLower, "lavenderblush") == 0) value = 0xfff0f5ff;
            else if (strcmp(colorStrLower, "lawngreen") == 0) value = 0x7cfc00ff;
            else if (strcmp(colorStrLower, "lemonchiffon") == 0) value = 0xfffacdff;
            else if (strcmp(colorStrLower, "lightblue") == 0) value = 0xadd8e6ff;
            else if (strcmp(colorStrLower, "lightcoral") == 0) value = 0xf08080ff;
            else if (strcmp(colorStrLower, "lightcyan") == 0) value = 0xe0ffffff;
            else if (strcmp(colorStrLower, "lightgoldenrodyellow") == 0) value = 0xfafad2ff;
            else if (strcmp(colorStrLower, "lightgray") == 0) value = 0xd3d3d3ff;
            else if (strcmp(colorStrLower, "lightgreen") == 0) value = 0x90ee90ff;
            else if (strcmp(colorStrLower, "lightgrey") == 0) value = 0xd3d3d3ff;
            else if (strcmp(colorStrLower, "lightpink") == 0) value = 0xffb6c1ff;
            else if (strcmp(colorStrLower, "lightsalmon") == 0) value = 0xffa07aff;
            else if (strcmp(colorStrLower, "lightseagreen") == 0) value = 0x20b2aaff;
            else if (strcmp(colorStrLower, "lightskyblue") == 0) value = 0x87cefaff;
            else if (strcmp(colorStrLower, "lightslategray") == 0) value = 0x778899ff;
            else if (strcmp(colorStrLower, "lightslategrey") == 0) value = 0x778899ff;
            else if (strcmp(colorStrLower, "lightsteelblue") == 0) value = 0xb0c4deff;
            else if (strcmp(colorStrLower, "lightyellow") == 0) value = 0xffffe0ff;
            else if (strcmp(colorStrLower, "limegreen") == 0) value = 0x32cd32ff;
            else if (strcmp(colorStrLower, "linen") == 0) value = 0xfaf0e6ff;
            else if (strcmp(colorStrLower, "magenta") == 0) value = 0xff00ffff;
            else if (strcmp(colorStrLower, "mediumaquamarine") == 0) value = 0x66cdaaff;
            else if (strcmp(colorStrLower, "mediumblue") == 0) value = 0x0000cdff;
            else if (strcmp(colorStrLower, "mediumorchid") == 0) value = 0xba55d3ff;
            else if (strcmp(colorStrLower, "mediumpurple") == 0) value = 0x9370dbff;
            else if (strcmp(colorStrLower, "mediumseagreen") == 0) value = 0x3cb371ff;
            else if (strcmp(colorStrLower, "mediumslateblue") == 0) value = 0x7b68eeff;
            else if (strcmp(colorStrLower, "mediumspringgreen") == 0) value = 0x00fa9aff;
            else if (strcmp(colorStrLower, "mediumturquoise") == 0) value = 0x48d1ccff;
            else if (strcmp(colorStrLower, "mediumvioletred") == 0) value = 0xc71585ff;
            else if (strcmp(colorStrLower, "midnightblue") == 0) value = 0x191970ff;
            else if (strcmp(colorStrLower, "mintcream") == 0) value = 0xf5fffaff;
            else if (strcmp(colorStrLower, "mistyrose") == 0) value = 0xffe4e1ff;
            else if (strcmp(colorStrLower, "moccasin") == 0) value = 0xffe4b5ff;
            else if (strcmp(colorStrLower, "navajowhite") == 0) value = 0xffdeadff;
            else if (strcmp(colorStrLower, "oldlace") == 0) value = 0xfdf5e6ff;
            else if (strcmp(colorStrLower, "olivedrab") == 0) value = 0x6b8e23ff;
            else if (strcmp(colorStrLower, "orangered") == 0) value = 0xff4500ff;
            else if (strcmp(colorStrLower, "orchid") == 0) value = 0xda70d6ff;
            else if (strcmp(colorStrLower, "palegoldenrod") == 0) value = 0xeee8aaff;
            else if (strcmp(colorStrLower, "palegreen") == 0) value = 0x98fb98ff;
            else if (strcmp(colorStrLower, "paleturquoise") == 0) value = 0xafeeeeff;
            else if (strcmp(colorStrLower, "palevioletred") == 0) value = 0xdb7093ff;
            else if (strcmp(colorStrLower, "papayawhip") == 0) value = 0xffefd5ff;
            else if (strcmp(colorStrLower, "peachpuff") == 0) value = 0xffdab9ff;
            else if (strcmp(colorStrLower, "peru") == 0) value = 0xcd853fff;
            else if (strcmp(colorStrLower, "pink") == 0) value = 0xffc0cbff;
            else if (strcmp(colorStrLower, "plum") == 0) value = 0xdda0ddff;
            else if (strcmp(colorStrLower, "powderblue") == 0) value = 0xb0e0e6ff;
            else if (strcmp(colorStrLower, "rosybrown") == 0) value = 0xbc8f8fff;
            else if (strcmp(colorStrLower, "royalblue") == 0) value = 0x4169e1ff;
            else if (strcmp(colorStrLower, "saddlebrown") == 0) value = 0x8b4513ff;
            else if (strcmp(colorStrLower, "salmon") == 0) value = 0xfa8072ff;
            else if (strcmp(colorStrLower, "sandybrown") == 0) value = 0xf4a460ff;
            else if (strcmp(colorStrLower, "seagreen") == 0) value = 0x2e8b57ff;
            else if (strcmp(colorStrLower, "seashell") == 0) value = 0xfff5eeff;
            else if (strcmp(colorStrLower, "sienna") == 0) value = 0xa0522dff;
            else if (strcmp(colorStrLower, "skyblue") == 0) value = 0x87ceebff;
            else if (strcmp(colorStrLower, "slateblue") == 0) value = 0x6a5acdff;
            else if (strcmp(colorStrLower, "slategray") == 0) value = 0x708090ff;
            else if (strcmp(colorStrLower, "slategrey") == 0) value = 0x708090ff;
            else if (strcmp(colorStrLower, "snow") == 0) value = 0xfffafaff;
            else if (strcmp(colorStrLower, "springgreen") == 0) value = 0x00ff7fff;
            else if (strcmp(colorStrLower, "steelblue") == 0) value = 0x4682b4ff;
            else if (strcmp(colorStrLower, "tan") == 0) value = 0xd2b48cff;
            else if (strcmp(colorStrLower, "thistle") == 0) value = 0xd8bfd8ff;
            else if (strcmp(colorStrLower, "tomato") == 0) value = 0xff6347ff;
            else if (strcmp(colorStrLower, "transparent") == 0) value = 0x00000000;
            else if (strcmp(colorStrLower, "turquoise") == 0) value = 0x40e0d0ff;
            else if (strcmp(colorStrLower, "violet") == 0) value = 0xee82eeff;
            else if (strcmp(colorStrLower, "wheat") == 0) value = 0xf5deb3ff;
            else if (strcmp(colorStrLower, "whitesmoke") == 0) value = 0xf5f5f5ff;
            else if (strcmp(colorStrLower, "yellowgreen") == 0) value = 0x9acd32ff;
        }
    }

    Color &Color::operator=(uint32_t colorVal) {
        Color((uint32_t) colorVal);
        return *this;
    }

    Color &Color::operator=(const char *colorStr) {
        Color((char *) colorStr);
        return *this;
    }

    Color &Color::operator=(const Color &other) {
        value = other.value;
        return *this;
    }

    double Color::red() {
        return (double) (value >> 24 & 0xff) / (double) 0xff;
    }

    double Color::green() {
        return (double) (value >> 16 & 0xff) / (double) 0xff;
    }

    double Color::blue() {
        return (double) (value >> 8 & 0xff) / (double) 0xff;
    }

    double Color::alpha() {
        return (double) (value & 0xff) / (double) 0xff;
    }

} // namespace ArtRobot
