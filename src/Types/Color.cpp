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

    Color::Color(uint32_t colorVal) : value(colorVal) {
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

            if (strcmp(colorStrLower, "maroon") == 0) value = Maroon.value;
            else if (strcmp(colorStrLower, "darkred") == 0) value = DarkRed.value;
            else if (strcmp(colorStrLower, "brown") == 0) value = Brown.value;
            else if (strcmp(colorStrLower, "firebrick") == 0) value = Firebrick.value;
            else if (strcmp(colorStrLower, "crimson") == 0) value = Crimson.value;
            else if (strcmp(colorStrLower, "red") == 0) value = Red.value;
            else if (strcmp(colorStrLower, "tomato") == 0) value = Tomato.value;
            else if (strcmp(colorStrLower, "coral") == 0) value = Coral.value;
            else if (strcmp(colorStrLower, "indianred") == 0) value = IndianRed.value;
            else if (strcmp(colorStrLower, "lightcoral") == 0) value = LightCoral.value;
            else if (strcmp(colorStrLower, "darksalmon") == 0) value = DarkSalmon.value;
            else if (strcmp(colorStrLower, "salmon") == 0) value = Salmon.value;
            else if (strcmp(colorStrLower, "lightsalmon") == 0) value = LightSalmon.value;
            else if (strcmp(colorStrLower, "orangered") == 0) value = OrangeRed.value;
            else if (strcmp(colorStrLower, "darkorange") == 0) value = DarkOrange.value;
            else if (strcmp(colorStrLower, "orange") == 0) value = Orange.value;
            else if (strcmp(colorStrLower, "gold") == 0) value = Gold.value;
            else if (strcmp(colorStrLower, "darkgoldenrod") == 0) value = DarkGoldenRod.value;
            else if (strcmp(colorStrLower, "goldenrod") == 0) value = GoldenRod.value;
            else if (strcmp(colorStrLower, "palegoldenrod") == 0) value = PaleGoldenRod.value;
            else if (strcmp(colorStrLower, "darkkhaki") == 0) value = DarkKhaki.value;
            else if (strcmp(colorStrLower, "khaki") == 0) value = Khaki.value;
            else if (strcmp(colorStrLower, "olive") == 0) value = Olive.value;
            else if (strcmp(colorStrLower, "yellow") == 0) value = Yellow.value;
            else if (strcmp(colorStrLower, "yellowgreen") == 0) value = YellowGreen.value;
            else if (strcmp(colorStrLower, "darkolivegreen") == 0) value = DarkOliveGreen.value;
            else if (strcmp(colorStrLower, "olivedrab") == 0) value = OliveDrab.value;
            else if (strcmp(colorStrLower, "lawngreen") == 0) value = LawnGreen.value;
            else if (strcmp(colorStrLower, "chartreuse") == 0) value = Chartreuse.value;
            else if (strcmp(colorStrLower, "greenyellow") == 0) value = GreenYellow.value;
            else if (strcmp(colorStrLower, "darkgreen") == 0) value = DarkGreen.value;
            else if (strcmp(colorStrLower, "green") == 0) value = Green.value;
            else if (strcmp(colorStrLower, "forestgreen") == 0) value = ForestGreen.value;
            else if (strcmp(colorStrLower, "lime") == 0) value = Lime.value;
            else if (strcmp(colorStrLower, "limegreen") == 0) value = LimeGreen.value;
            else if (strcmp(colorStrLower, "lightgreen") == 0) value = LightGreen.value;
            else if (strcmp(colorStrLower, "palegreen") == 0) value = PaleGreen.value;
            else if (strcmp(colorStrLower, "darkseagreen") == 0) value = DarkSeaGreen.value;
            else if (strcmp(colorStrLower, "mediumspringgreen") == 0) value = MediumSpringGreen.value;
            else if (strcmp(colorStrLower, "springgreen") == 0) value = SpringGreen.value;
            else if (strcmp(colorStrLower, "seagreen") == 0) value = SeaGreen.value;
            else if (strcmp(colorStrLower, "mediumaquamarine") == 0) value = MediumAquaMarine.value;
            else if (strcmp(colorStrLower, "mediumseagreen") == 0) value = MediumSeaGreen.value;
            else if (strcmp(colorStrLower, "lightseagreen") == 0) value = LightSeaGreen.value;
            else if (strcmp(colorStrLower, "darkslategray") == 0) value = DarkSlateGray.value;
            else if (strcmp(colorStrLower, "darkslategrey") == 0) value = DarkSlateGrey.value;
            else if (strcmp(colorStrLower, "teal") == 0) value = Teal.value;
            else if (strcmp(colorStrLower, "darkcyan") == 0) value = DarkCyan.value;
            else if (strcmp(colorStrLower, "aqua") == 0) value = Aqua.value;
            else if (strcmp(colorStrLower, "cyan") == 0) value = Cyan.value;
            else if (strcmp(colorStrLower, "lightcyan") == 0) value = LightCyan.value;
            else if (strcmp(colorStrLower, "darkturquoise") == 0) value = DarkTurquoise.value;
            else if (strcmp(colorStrLower, "turquoise") == 0) value = Turquoise.value;
            else if (strcmp(colorStrLower, "mediumturquoise") == 0) value = MediumTurquoise.value;
            else if (strcmp(colorStrLower, "paleturquoise") == 0) value = PaleTurquoise.value;
            else if (strcmp(colorStrLower, "aquamarine") == 0) value = AquaMarine.value;
            else if (strcmp(colorStrLower, "powderblue") == 0) value = PowderBlue.value;
            else if (strcmp(colorStrLower, "cadetblue") == 0) value = CadetBlue.value;
            else if (strcmp(colorStrLower, "steelblue") == 0) value = SteelBlue.value;
            else if (strcmp(colorStrLower, "cornflowerblue") == 0) value = CornFlowerBlue.value;
            else if (strcmp(colorStrLower, "deepskyblue") == 0) value = DeepSkyBlue.value;
            else if (strcmp(colorStrLower, "dodgerblue") == 0) value = DodgerBlue.value;
            else if (strcmp(colorStrLower, "lightblue") == 0) value = LightBlue.value;
            else if (strcmp(colorStrLower, "skyblue") == 0) value = SkyBlue.value;
            else if (strcmp(colorStrLower, "lightskyblue") == 0) value = LightSkyBlue.value;
            else if (strcmp(colorStrLower, "midnightblue") == 0) value = MidnightBlue.value;
            else if (strcmp(colorStrLower, "navy") == 0) value = Navy.value;
            else if (strcmp(colorStrLower, "darkblue") == 0) value = DarkBlue.value;
            else if (strcmp(colorStrLower, "mediumblue") == 0) value = MediumBlue.value;
            else if (strcmp(colorStrLower, "blue") == 0) value = Blue.value;
            else if (strcmp(colorStrLower, "royalblue") == 0) value = RoyalBlue.value;
            else if (strcmp(colorStrLower, "blueviolet") == 0) value = BlueViolet.value;
            else if (strcmp(colorStrLower, "indigo") == 0) value = Indigo.value;
            else if (strcmp(colorStrLower, "darkslateblue") == 0) value = DarkSlateBlue.value;
            else if (strcmp(colorStrLower, "slateblue") == 0) value = SlateBlue.value;
            else if (strcmp(colorStrLower, "mediumslateblue") == 0) value = MediumSlateBlue.value;
            else if (strcmp(colorStrLower, "mediumpurple") == 0) value = MediumPurple.value;
            else if (strcmp(colorStrLower, "darkmagenta") == 0) value = DarkMagenta.value;
            else if (strcmp(colorStrLower, "darkviolet") == 0) value = DarkViolet.value;
            else if (strcmp(colorStrLower, "darkorchid") == 0) value = DarkOrchid.value;
            else if (strcmp(colorStrLower, "mediumorchid") == 0) value = MediumOrchid.value;
            else if (strcmp(colorStrLower, "purple") == 0) value = Purple.value;
            else if (strcmp(colorStrLower, "thistle") == 0) value = Thistle.value;
            else if (strcmp(colorStrLower, "plum") == 0) value = Plum.value;
            else if (strcmp(colorStrLower, "violet") == 0) value = Violet.value;
            else if (strcmp(colorStrLower, "magenta") == 0) value = Magenta.value;
            else if (strcmp(colorStrLower, "fuchsia") == 0) value = Fuchsia.value;
            else if (strcmp(colorStrLower, "orchid") == 0) value = Orchid.value;
            else if (strcmp(colorStrLower, "mediumvioletred") == 0) value = MediumVioletRed.value;
            else if (strcmp(colorStrLower, "palevioletred") == 0) value = PaleVioletRed.value;
            else if (strcmp(colorStrLower, "deeppink") == 0) value = DeepPink.value;
            else if (strcmp(colorStrLower, "hotpink") == 0) value = HotPink.value;
            else if (strcmp(colorStrLower, "lightpink") == 0) value = LightPink.value;
            else if (strcmp(colorStrLower, "pink") == 0) value = Pink.value;
            else if (strcmp(colorStrLower, "antiquewhite") == 0) value = AntiqueWhite.value;
            else if (strcmp(colorStrLower, "beige") == 0) value = Beige.value;
            else if (strcmp(colorStrLower, "bisque") == 0) value = Bisque.value;
            else if (strcmp(colorStrLower, "blanchedalmond") == 0) value = BlanchedAlmond.value;
            else if (strcmp(colorStrLower, "wheat") == 0) value = Wheat.value;
            else if (strcmp(colorStrLower, "cornsilk") == 0) value = CornSilk.value;
            else if (strcmp(colorStrLower, "lemonchiffon") == 0) value = LemonChiffon.value;
            else if (strcmp(colorStrLower, "lightgoldenrodyellow") == 0) value = LightGoldenRodYellow.value;
            else if (strcmp(colorStrLower, "lightyellow") == 0) value = LightYellow.value;
            else if (strcmp(colorStrLower, "saddlebrown") == 0) value = SaddleBrown.value;
            else if (strcmp(colorStrLower, "sienna") == 0) value = Sienna.value;
            else if (strcmp(colorStrLower, "chocolate") == 0) value = Chocolate.value;
            else if (strcmp(colorStrLower, "peru") == 0) value = Peru.value;
            else if (strcmp(colorStrLower, "sandybrown") == 0) value = SandyBrown.value;
            else if (strcmp(colorStrLower, "burlywood") == 0) value = BurlyWood.value;
            else if (strcmp(colorStrLower, "tan") == 0) value = Tan.value;
            else if (strcmp(colorStrLower, "rosybrown") == 0) value = RosyBrown.value;
            else if (strcmp(colorStrLower, "moccasin") == 0) value = Moccasin.value;
            else if (strcmp(colorStrLower, "navajowhite") == 0) value = NavajoWhite.value;
            else if (strcmp(colorStrLower, "peachpuff") == 0) value = PeachPuff.value;
            else if (strcmp(colorStrLower, "mistyrose") == 0) value = MistyRose.value;
            else if (strcmp(colorStrLower, "lavenderblush") == 0) value = LavenderBlush.value;
            else if (strcmp(colorStrLower, "linen") == 0) value = Linen.value;
            else if (strcmp(colorStrLower, "oldlace") == 0) value = OldLace.value;
            else if (strcmp(colorStrLower, "papayawhip") == 0) value = PapayaWhip.value;
            else if (strcmp(colorStrLower, "seashell") == 0) value = SeaShell.value;
            else if (strcmp(colorStrLower, "mintcream") == 0) value = MintCream.value;
            else if (strcmp(colorStrLower, "slategray") == 0) value = SlateGray.value;
            else if (strcmp(colorStrLower, "slategrey") == 0) value = SlateGrey.value;
            else if (strcmp(colorStrLower, "lightslategray") == 0) value = LightSlateGray.value;
            else if (strcmp(colorStrLower, "lightslategrey") == 0) value = LightSlateGrey.value;
            else if (strcmp(colorStrLower, "lightsteelblue") == 0) value = LightSteelBlue.value;
            else if (strcmp(colorStrLower, "lavender") == 0) value = Lavender.value;
            else if (strcmp(colorStrLower, "floralwhite") == 0) value = FloralWhite.value;
            else if (strcmp(colorStrLower, "aliceblue") == 0) value = AliceBlue.value;
            else if (strcmp(colorStrLower, "ghostwhite") == 0) value = GhostWhite.value;
            else if (strcmp(colorStrLower, "honeydew") == 0) value = Honeydew.value;
            else if (strcmp(colorStrLower, "ivory") == 0) value = Ivory.value;
            else if (strcmp(colorStrLower, "azure") == 0) value = Azure.value;
            else if (strcmp(colorStrLower, "snow") == 0) value = Snow.value;
            else if (strcmp(colorStrLower, "black") == 0) value = Black.value;
            else if (strcmp(colorStrLower, "dimgray") == 0) value = DimGray.value;
            else if (strcmp(colorStrLower, "dimgrey") == 0) value = DimGrey.value;
            else if (strcmp(colorStrLower, "gray") == 0) value = Gray.value;
            else if (strcmp(colorStrLower, "grey") == 0) value = Grey.value;
            else if (strcmp(colorStrLower, "darkgray") == 0) value = DarkGray.value;
            else if (strcmp(colorStrLower, "darkgrey") == 0) value = DarkGrey.value;
            else if (strcmp(colorStrLower, "silver") == 0) value = Silver.value;
            else if (strcmp(colorStrLower, "lightgray") == 0) value = LightGray.value;
            else if (strcmp(colorStrLower, "lightgrey") == 0) value = LightGrey.value;
            else if (strcmp(colorStrLower, "gainsboro") == 0) value = Gainsboro.value;
            else if (strcmp(colorStrLower, "whitesmoke") == 0) value = WhiteSmoke.value;
            else if (strcmp(colorStrLower, "white") == 0) value = White.value;
            else if (strcmp(colorStrLower, "transparent") == 0) value = Transparent.value;
            else value = 0x000000FF; // 黑不透明
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

//    const Color Color::Black{uint32_t(0x000000FF)};                 // ( 0,   0,   0,   255 )
//    const Color Color::White{uint32_t(0xFFFFFFFF)};                 // ( 255, 255, 255, 255 )
//    const Color Color::Red{uint32_t(0xFF0000FF)};                   // ( 255, 0,   0,   255 )
//    const Color Color::Lime{uint32_t(0x00FF00FF)};                  // ( 0,   255, 0,   255 )
//    const Color Color::Blue{uint32_t(0x0000FFFF)};                  // ( 0,   0,   255, 255 )
//    const Color Color::Yellow{uint32_t(0xFFFF00FF)};                // ( 255, 255, 0,   255 )
//    const Color Color::Aqua{uint32_t(0x00FFFFFF)};                  // ( 0,   255, 255, 255 )
//    const Color Color::Fuchsia{uint32_t(0xFF00FFFF)};               // ( 255, 0,   255, 255 )
//    const Color Color::Silver{uint32_t(0xC0C0C0FF)};                // ( 192, 192, 192, 255 )
//    const Color Color::Gray{uint32_t(0x808080FF)};                  // ( 128, 128, 128, 255 )
//    const Color Color::Maroon{uint32_t(0x800000FF)};                // ( 128, 0,   0,   255 )
//    const Color Color::Olive{uint32_t(0x808000FF)};                 // ( 128, 128, 0,   255 )
//    const Color Color::Green{uint32_t(0x008000FF)};                 // ( 0,   128, 0,   255 )
//    const Color Color::Purple{uint32_t(0x800080FF)};                // ( 128, 0,   128, 255 )
//    const Color Color::Teal{uint32_t(0x008080FF)};                  // ( 0,   128, 128, 255 )
//    const Color Color::Navy{uint32_t(0x000080FF)};                  // ( 0,   0,   128, 255 )

    const Color Color::Maroon{uint32_t(0x800000FF)};                // ( 128, 0,   0,   255 )
    const Color Color::DarkRed{uint32_t(0x8B0000FF)};               // ( 139, 0,   0,   255 )
    const Color Color::Brown{uint32_t(0xA52A2AFF)};                 // ( 165, 42,  42,  255 )
    const Color Color::Firebrick{uint32_t(0xB22222FF)};             // ( 178, 34,  34,  255 )
    const Color Color::Crimson{uint32_t(0xDC143CFF)};               // ( 220, 20,  60,  255 )
    const Color Color::Red{uint32_t(0xFF0000FF)};                   // ( 255, 0,   0,   255 )
    const Color Color::Tomato{uint32_t(0xFF6347FF)};                // ( 255, 99,  71,  255 )
    const Color Color::Coral{uint32_t(0xFF7F50FF)};                 // ( 255, 127, 80,  255 )
    const Color Color::IndianRed{uint32_t(0xCD5C5CFF)};             // ( 205, 92,  92,  255 )
    const Color Color::LightCoral{uint32_t(0xF08080FF)};            // ( 240, 128, 128, 255 )
    const Color Color::DarkSalmon{uint32_t(0xE9967AFF)};            // ( 233, 150, 122, 255 )
    const Color Color::Salmon{uint32_t(0xFA8072FF)};                // ( 250, 128, 114, 255 )
    const Color Color::LightSalmon{uint32_t(0xFFA07AFF)};           // ( 255, 160, 122, 255 )
    const Color Color::OrangeRed{uint32_t(0xFF4500FF)};             // ( 255, 69,  0,   255 )
    const Color Color::DarkOrange{uint32_t(0xFF8C00FF)};            // ( 255, 140, 0,   255 )
    const Color Color::Orange{uint32_t(0xFFA500FF)};                // ( 255, 165, 0,   255 )
    const Color Color::Gold{uint32_t(0xFFD700FF)};                  // ( 255, 215, 0,   255 )
    const Color Color::DarkGoldenRod{uint32_t(0xB8860BFF)};         // ( 184, 134, 11,  255 )
    const Color Color::GoldenRod{uint32_t(0xDAA520FF)};             // ( 218, 165, 32,  255 )
    const Color Color::PaleGoldenRod{uint32_t(0xEEE8AAFF)};         // ( 238, 232, 170, 255 )
    const Color Color::DarkKhaki{uint32_t(0xBDB76BFF)};             // ( 189, 183, 107, 255 )
    const Color Color::Khaki{uint32_t(0xF0E68CFF)};                 // ( 240, 230, 140, 255 )
    const Color Color::Olive{uint32_t(0x808000FF)};                 // ( 128, 128, 0,   255 )
    const Color Color::Yellow{uint32_t(0xFFFF00FF)};                // ( 255, 255, 0,   255 )
    const Color Color::YellowGreen{uint32_t(0x9ACD32FF)};           // ( 154, 205, 50,  255 )
    const Color Color::DarkOliveGreen{uint32_t(0x556B2FFF)};        // ( 85,  107, 47,  255 )
    const Color Color::OliveDrab{uint32_t(0x6B8E23FF)};             // ( 107, 142, 35,  255 )
    const Color Color::LawnGreen{uint32_t(0x7CFC00FF)};             // ( 124, 252, 0,   255 )
    const Color Color::Chartreuse{uint32_t(0x7FFF00FF)};            // ( 127, 255, 0,   255 )
    const Color Color::GreenYellow{uint32_t(0xADFF2FFF)};           // ( 173, 255, 47,  255 )
    const Color Color::DarkGreen{uint32_t(0x006400FF)};             // ( 0,   100, 0,   255 )
    const Color Color::Green{uint32_t(0x008000FF)};                 // ( 0,   128, 0,   255 )
    const Color Color::ForestGreen{uint32_t(0x228B22FF)};           // ( 34,  139, 34,  255 )
    const Color Color::Lime{uint32_t(0x00FF00FF)};                  // ( 0,   255, 0,   255 )
    const Color Color::LimeGreen{uint32_t(0x32CD32FF)};             // ( 50,  205, 50,  255 )
    const Color Color::LightGreen{uint32_t(0x90EE90FF)};            // ( 144, 238, 144, 255 )
    const Color Color::PaleGreen{uint32_t(0x98FB98FF)};             // ( 152, 251, 152, 255 )
    const Color Color::DarkSeaGreen{uint32_t(0x8FBC8FFF)};          // ( 143, 188, 143, 255 )
    const Color Color::MediumSpringGreen{uint32_t(0x00FA9AFF)};     // ( 0,   250, 154, 255 )
    const Color Color::SpringGreen{uint32_t(0x00FF7FFF)};           // ( 0,   255, 127, 255 )
    const Color Color::SeaGreen{uint32_t(0x2E8B57FF)};              // ( 46,  139, 87,  255 )
    const Color Color::MediumAquaMarine{uint32_t(0x66CDAAFF)};      // ( 102, 205, 170, 255 )
    const Color Color::MediumSeaGreen{uint32_t(0x3CB371FF)};        // ( 60,  179, 113, 255 )
    const Color Color::LightSeaGreen{uint32_t(0x20B2AAFF)};         // ( 32,  178, 170, 255 )
    const Color Color::DarkSlateGray{uint32_t(0x2F4F4FFF)};         // ( 47,  79,  79,  255 )
    const Color Color::DarkSlateGrey{uint32_t(0x2F4F4FFF)};         // ( 47,  79,  79,  255 )
    const Color Color::Teal{uint32_t(0x008080FF)};                  // ( 0,   128, 128, 255 )
    const Color Color::DarkCyan{uint32_t(0x008B8BFF)};              // ( 0,   139, 139, 255 )
    const Color Color::Aqua{uint32_t(0x00FFFFFF)};                  // ( 0,   255, 255, 255 )
    const Color Color::Cyan{uint32_t(0x00FFFFFF)};                  // ( 0,   255, 255, 255 )
    const Color Color::LightCyan{uint32_t(0xE0FFFFFF)};             // ( 224, 255, 255, 255 )
    const Color Color::DarkTurquoise{uint32_t(0x00CED1FF)};         // ( 0,   206, 209, 255 )
    const Color Color::Turquoise{uint32_t(0x40E0D0FF)};             // ( 64,  224, 208, 255 )
    const Color Color::MediumTurquoise{uint32_t(0x48D1CCFF)};       // ( 72,  209, 204, 255 )
    const Color Color::PaleTurquoise{uint32_t(0xAFEEEEFF)};         // ( 175, 238, 238, 255 )
    const Color Color::AquaMarine{uint32_t(0x7FFFD4FF)};            // ( 127, 255, 212, 255 )
    const Color Color::PowderBlue{uint32_t(0xB0E0E6FF)};            // ( 176, 224, 230, 255 )
    const Color Color::CadetBlue{uint32_t(0x5F9EA0FF)};             // ( 95,  158, 160, 255 )
    const Color Color::SteelBlue{uint32_t(0x4682B4FF)};             // ( 70,  130, 180, 255 )
    const Color Color::CornFlowerBlue{uint32_t(0x6495EDFF)};        // ( 100, 149, 237, 255 )
    const Color Color::DeepSkyBlue{uint32_t(0x00BFFFFF)};           // ( 0,   191, 255, 255 )
    const Color Color::DodgerBlue{uint32_t(0x1E90FFFF)};            // ( 30,  144, 255, 255 )
    const Color Color::LightBlue{uint32_t(0xADD8E6FF)};             // ( 173, 216, 230, 255 )
    const Color Color::SkyBlue{uint32_t(0x87CEEBFF)};               // ( 135, 206, 235, 255 )
    const Color Color::LightSkyBlue{uint32_t(0x87CEFAFF)};          // ( 135, 206, 250, 255 )
    const Color Color::MidnightBlue{uint32_t(0x191970FF)};          // ( 25,  25,  112, 255 )
    const Color Color::Navy{uint32_t(0x000080FF)};                  // ( 0,   0,   128, 255 )
    const Color Color::DarkBlue{uint32_t(0x00008BFF)};              // ( 0,   0,   139, 255 )
    const Color Color::MediumBlue{uint32_t(0x0000CDFF)};            // ( 0,   0,   205, 255 )
    const Color Color::Blue{uint32_t(0x0000FFFF)};                  // ( 0,   0,   255, 255 )
    const Color Color::RoyalBlue{uint32_t(0x4169E1FF)};             // ( 65,  105, 225, 255 )
    const Color Color::BlueViolet{uint32_t(0x8A2BE2FF)};            // ( 138, 43,  226, 255 )
    const Color Color::Indigo{uint32_t(0x4B0082FF)};                // ( 75,  0,   130, 255 )
    const Color Color::DarkSlateBlue{uint32_t(0x483D8BFF)};         // ( 72,  61,  139, 255 )
    const Color Color::SlateBlue{uint32_t(0x6A5ACDFF)};             // ( 106, 90,  205, 255 )
    const Color Color::MediumSlateBlue{uint32_t(0x7B68EEFF)};       // ( 123, 104, 238, 255 )
    const Color Color::MediumPurple{uint32_t(0x9370DBFF)};          // ( 147, 112, 219, 255 )
    const Color Color::DarkMagenta{uint32_t(0x8B008BFF)};           // ( 139, 0,   139, 255 )
    const Color Color::DarkViolet{uint32_t(0x9400D3FF)};            // ( 148, 0,   211, 255 )
    const Color Color::DarkOrchid{uint32_t(0x9932CCFF)};            // ( 153, 50,  204, 255 )
    const Color Color::MediumOrchid{uint32_t(0xBA55D3FF)};          // ( 186, 85,  211, 255 )
    const Color Color::Purple{uint32_t(0x800080FF)};                // ( 128, 0,   128, 255 )
    const Color Color::Thistle{uint32_t(0xD8BFD8FF)};               // ( 216, 191, 216, 255 )
    const Color Color::Plum{uint32_t(0xDDA0DDFF)};                  // ( 221, 160, 221, 255 )
    const Color Color::Violet{uint32_t(0xEE82EEFF)};                // ( 238, 130, 238, 255 )
    const Color Color::Magenta{uint32_t(0xFF00FFFF)};               // ( 255, 0,   255, 255 )
    const Color Color::Fuchsia{uint32_t(0xFF00FFFF)};               // ( 255, 0,   255, 255 )
    const Color Color::Orchid{uint32_t(0xDA70D6FF)};                // ( 218, 112, 214, 255 )
    const Color Color::MediumVioletRed{uint32_t(0xC71585FF)};       // ( 199, 21,  133, 255 )
    const Color Color::PaleVioletRed{uint32_t(0xDB7093FF)};         // ( 219, 112, 147, 255 )
    const Color Color::DeepPink{uint32_t(0xFF1493FF)};              // ( 255, 20,  147, 255 )
    const Color Color::HotPink{uint32_t(0xFF69B4FF)};               // ( 255, 105, 180, 255 )
    const Color Color::LightPink{uint32_t(0xFFB6C1FF)};             // ( 255, 182, 193, 255 )
    const Color Color::Pink{uint32_t(0xFFC0CBFF)};                  // ( 255, 192, 203, 255 )
    const Color Color::AntiqueWhite{uint32_t(0xFAEBD7FF)};          // ( 250, 235, 215, 255 )
    const Color Color::Beige{uint32_t(0xF5F5DCFF)};                 // ( 245, 245, 220, 255 )
    const Color Color::Bisque{uint32_t(0xFFE4C4FF)};                // ( 255, 228, 196, 255 )
    const Color Color::BlanchedAlmond{uint32_t(0xFFEBCDFF)};        // ( 255, 235, 205, 255 )
    const Color Color::Wheat{uint32_t(0xF5DEB3FF)};                 // ( 245, 222, 179, 255 )
    const Color Color::CornSilk{uint32_t(0xFFF8DCFF)};              // ( 255, 248, 220, 255 )
    const Color Color::LemonChiffon{uint32_t(0xFFFACDFF)};          // ( 255, 250, 205, 255 )
    const Color Color::LightGoldenRodYellow{uint32_t(0xFAFAD2FF)};  // ( 250, 250, 210, 255 )
    const Color Color::LightYellow{uint32_t(0xFFFFE0FF)};           // ( 255, 255, 224, 255 )
    const Color Color::SaddleBrown{uint32_t(0x8B4513FF)};           // ( 139, 69,  19,  255 )
    const Color Color::Sienna{uint32_t(0xA0522DFF)};                // ( 160, 82,  45,  255 )
    const Color Color::Chocolate{uint32_t(0xD2691EFF)};             // ( 210, 105, 30,  255 )
    const Color Color::Peru{uint32_t(0xCD853FFF)};                  // ( 205, 133, 63,  255 )
    const Color Color::SandyBrown{uint32_t(0xF4A460FF)};            // ( 244, 164, 96,  255 )
    const Color Color::BurlyWood{uint32_t(0xDEB887FF)};             // ( 222, 184, 135, 255 )
    const Color Color::Tan{uint32_t(0xD2B48CFF)};                   // ( 210, 180, 140, 255 )
    const Color Color::RosyBrown{uint32_t(0xBC8F8FFF)};             // ( 188, 143, 143, 255 )
    const Color Color::Moccasin{uint32_t(0xFFE4B5FF)};              // ( 255, 228, 181, 255 )
    const Color Color::NavajoWhite{uint32_t(0xFFDEADFF)};           // ( 255, 222, 173, 255 )
    const Color Color::PeachPuff{uint32_t(0xFFDAB9FF)};             // ( 255, 218, 185, 255 )
    const Color Color::MistyRose{uint32_t(0xFFE4E1FF)};             // ( 255, 228, 225, 255 )
    const Color Color::LavenderBlush{uint32_t(0xFFF0F5FF)};         // ( 255, 240, 245, 255 )
    const Color Color::Linen{uint32_t(0xFAF0E6FF)};                 // ( 250, 240, 230, 255 )
    const Color Color::OldLace{uint32_t(0xFDF5E6FF)};               // ( 253, 245, 230, 255 )
    const Color Color::PapayaWhip{uint32_t(0xFFEFD5FF)};            // ( 255, 239, 213, 255 )
    const Color Color::SeaShell{uint32_t(0xFFF5EEFF)};              // ( 255, 245, 238, 255 )
    const Color Color::MintCream{uint32_t(0xF5FFFAFF)};             // ( 245, 255, 250, 255 )
    const Color Color::SlateGray{uint32_t(0x708090FF)};             // ( 112, 128, 144, 255 )
    const Color Color::SlateGrey{uint32_t(0x708090FF)};             // ( 112, 128, 144, 255 )
    const Color Color::LightSlateGray{uint32_t(0x778899FF)};        // ( 119, 136, 153, 255 )
    const Color Color::LightSlateGrey{uint32_t(0x778899FF)};        // ( 119, 136, 153, 255 )
    const Color Color::LightSteelBlue{uint32_t(0xB0C4DEFF)};        // ( 176, 196, 222, 255 )
    const Color Color::Lavender{uint32_t(0xE6E6FAFF)};              // ( 230, 230, 250, 255 )
    const Color Color::FloralWhite{uint32_t(0xFFFAF0FF)};           // ( 255, 250, 240, 255 )
    const Color Color::AliceBlue{uint32_t(0xF0F8FFFF)};             // ( 240, 248, 255, 255 )
    const Color Color::GhostWhite{uint32_t(0xF8F8FFFF)};            // ( 248, 248, 255, 255 )
    const Color Color::Honeydew{uint32_t(0xF0FFF0FF)};              // ( 240, 255, 240, 255 )
    const Color Color::Ivory{uint32_t(0xFFFFF0FF)};                 // ( 255, 255, 240, 255 )
    const Color Color::Azure{uint32_t(0xF0FFFFFF)};                 // ( 240, 255, 255, 255 )
    const Color Color::Snow{uint32_t(0xFFFAFAFF)};                  // ( 255, 250, 250, 255 )
    const Color Color::Black{uint32_t(0x000000FF)};                 // ( 0,   0,   0,   255 )
    const Color Color::DimGray{uint32_t(0x696969FF)};               // ( 105, 105, 105, 255 )
    const Color Color::DimGrey{uint32_t(0x696969FF)};               // ( 105, 105, 105, 255 )
    const Color Color::Gray{uint32_t(0x808080FF)};                  // ( 128, 128, 128, 255 )
    const Color Color::Grey{uint32_t(0x808080FF)};                  // ( 128, 128, 128, 255 )
    const Color Color::DarkGray{uint32_t(0xA9A9A9FF)};              // ( 169, 169, 169, 255 )
    const Color Color::DarkGrey{uint32_t(0xA9A9A9FF)};              // ( 169, 169, 169, 255 )
    const Color Color::Silver{uint32_t(0xC0C0C0FF)};                // ( 192, 192, 192, 255 )
    const Color Color::LightGray{uint32_t(0xD3D3D3FF)};             // ( 211, 211, 211, 255 )
    const Color Color::LightGrey{uint32_t(0xD3D3D3FF)};             // ( 211, 211, 211, 255 )
    const Color Color::Gainsboro{uint32_t(0xDCDCDCFF)};             // ( 220, 220, 220, 255 )
    const Color Color::WhiteSmoke{uint32_t(0xF5F5F5FF)};            // ( 245, 245, 245, 255 )
    const Color Color::White{uint32_t(0xFFFFFFFF)};                 // ( 255, 255, 255, 255 )
    const Color Color::Transparent{uint32_t(0x00000000)};           // ( 0,   0,   0,   0   )

} // namespace ArtRobot
