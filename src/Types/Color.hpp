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

#include <cstdint>

namespace ArtRobot {

    class Color {
    private:
        union Value {
            uint32_t v;
#ifdef _MSC_VER
            __pragma( pack(push, 1) )
#endif
            struct {
                uint8_t a;
                uint8_t b;
                uint8_t g;
                uint8_t r;
            }
#ifdef __GNUC__
                __attribute__((__packed__))
#endif
#ifdef _MSC_VER
            __pragma( pack(pop))
#endif
                    rgba;
        } value;


    public:
        Color();

        Color(uint32_t colorVal);

        Color(const char *colorStr);

        Color &operator=(uint32_t colorVal);

        Color &operator=(const char *colorStr);

        Color &operator=(const Color &other);

        inline double r() {
            return (double) (value.rgba.r) / (double) 0xff;
        }

        inline double g() {
            return (double) (value.rgba.g) / (double) 0xff;
        }

        inline double b() {
            return (double) (value.rgba.b) / (double) 0xff;
        }

        inline double a() {
            return (double) (value.rgba.a) / (double) 0xff;
        }

        const static Color Maroon;
        const static Color DarkRed;
        const static Color Brown;
        const static Color Firebrick;
        const static Color Crimson;
        const static Color Red;
        const static Color Tomato;
        const static Color Coral;
        const static Color IndianRed;
        const static Color LightCoral;
        const static Color DarkSalmon;
        const static Color Salmon;
        const static Color LightSalmon;
        const static Color OrangeRed;
        const static Color DarkOrange;
        const static Color Orange;
        const static Color Gold;
        const static Color DarkGoldenRod;
        const static Color GoldenRod;
        const static Color PaleGoldenRod;
        const static Color DarkKhaki;
        const static Color Khaki;
        const static Color Olive;
        const static Color Yellow;
        const static Color YellowGreen;
        const static Color DarkOliveGreen;
        const static Color OliveDrab;
        const static Color LawnGreen;
        const static Color Chartreuse;
        const static Color GreenYellow;
        const static Color DarkGreen;
        const static Color Green;
        const static Color ForestGreen;
        const static Color Lime;
        const static Color LimeGreen;
        const static Color LightGreen;
        const static Color PaleGreen;
        const static Color DarkSeaGreen;
        const static Color MediumSpringGreen;
        const static Color SpringGreen;
        const static Color SeaGreen;
        const static Color MediumAquaMarine;
        const static Color MediumSeaGreen;
        const static Color LightSeaGreen;
        const static Color DarkSlateGray;
        const static Color DarkSlateGrey;
        const static Color Teal;
        const static Color DarkCyan;
        const static Color Aqua;
        const static Color Cyan;
        const static Color LightCyan;
        const static Color DarkTurquoise;
        const static Color Turquoise;
        const static Color MediumTurquoise;
        const static Color PaleTurquoise;
        const static Color AquaMarine;
        const static Color PowderBlue;
        const static Color CadetBlue;
        const static Color SteelBlue;
        const static Color CornFlowerBlue;
        const static Color DeepSkyBlue;
        const static Color DodgerBlue;
        const static Color LightBlue;
        const static Color SkyBlue;
        const static Color LightSkyBlue;
        const static Color MidnightBlue;
        const static Color Navy;
        const static Color DarkBlue;
        const static Color MediumBlue;
        const static Color Blue;
        const static Color RoyalBlue;
        const static Color BlueViolet;
        const static Color Indigo;
        const static Color DarkSlateBlue;
        const static Color SlateBlue;
        const static Color MediumSlateBlue;
        const static Color MediumPurple;
        const static Color DarkMagenta;
        const static Color DarkViolet;
        const static Color DarkOrchid;
        const static Color MediumOrchid;
        const static Color Purple;
        const static Color Thistle;
        const static Color Plum;
        const static Color Violet;
        const static Color Magenta;
        const static Color Fuchsia;
        const static Color Orchid;
        const static Color MediumVioletRed;
        const static Color PaleVioletRed;
        const static Color DeepPink;
        const static Color HotPink;
        const static Color LightPink;
        const static Color Pink;
        const static Color AntiqueWhite;
        const static Color Beige;
        const static Color Bisque;
        const static Color BlanchedAlmond;
        const static Color Wheat;
        const static Color CornSilk;
        const static Color LemonChiffon;
        const static Color LightGoldenRodYellow;
        const static Color LightYellow;
        const static Color SaddleBrown;
        const static Color Sienna;
        const static Color Chocolate;
        const static Color Peru;
        const static Color SandyBrown;
        const static Color BurlyWood;
        const static Color Tan;
        const static Color RosyBrown;
        const static Color Moccasin;
        const static Color NavajoWhite;
        const static Color PeachPuff;
        const static Color MistyRose;
        const static Color LavenderBlush;
        const static Color Linen;
        const static Color OldLace;
        const static Color PapayaWhip;
        const static Color SeaShell;
        const static Color MintCream;
        const static Color SlateGray;
        const static Color SlateGrey;
        const static Color LightSlateGray;
        const static Color LightSlateGrey;
        const static Color LightSteelBlue;
        const static Color Lavender;
        const static Color FloralWhite;
        const static Color AliceBlue;
        const static Color GhostWhite;
        const static Color Honeydew;
        const static Color Ivory;
        const static Color Azure;
        const static Color Snow;
        const static Color Black;
        const static Color DimGray;
        const static Color DimGrey;
        const static Color Gray;
        const static Color Grey;
        const static Color DarkGray;
        const static Color DarkGrey;
        const static Color Silver;
        const static Color LightGray;
        const static Color LightGrey;
        const static Color Gainsboro;
        const static Color WhiteSmoke;
        const static Color White;
        const static Color Transparent;

    };

} // namespace ArtRobot
