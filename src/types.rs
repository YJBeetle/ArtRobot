pub struct Property {
    pub name: String,
    pub width: f64,
    pub height: f64,
}

impl Default for Property {
    fn default() -> Self {
        Self {
            name: String::new(),
            width: 0.,
            height: 0.,
        }
    }
}

pub enum Anchor {
    LT,
    CT,
    RT,

    LC,
    CC,
    RC,

    LD,
    CD,
    RD,
}

pub struct Transform {
    pub x: f64,
    pub y: f64,
    pub rotate: f64,
    pub anchor: Anchor,
    pub scale_x: f64,
    pub scale_y: f64,
}

impl Default for Transform {
    fn default() -> Self {
        Self {
            x: 0.,
            y: 0.,
            rotate: 0.,
            anchor: Anchor::CC,
            scale_x: 1.,
            scale_y: 1.,
        }
    }
}

impl Transform {
    pub fn apply(&self, property: &Property, cr: &cairo::Context) {
        use std::f64::consts::PI;
        cr.translate(self.x, self.y);
        cr.rotate(self.rotate * PI / 180.);
        cr.scale(self.scale_x, self.scale_y);
        match self.anchor {
            Anchor::LT => {}
            Anchor::CT => cr.translate(-property.width / 2., 0.),
            Anchor::RT => cr.translate(-property.width, 0.),
            Anchor::LC => cr.translate(0., -property.height / 2.),
            Anchor::CC => cr.translate(-property.width / 2., -property.height / 2.),
            Anchor::RC => cr.translate(-property.width, -property.height / 2.),
            Anchor::LD => cr.translate(0., -property.height),
            Anchor::CD => cr.translate(-property.width / 2., -property.height),
            Anchor::RD => cr.translate(-property.width, -property.height),
        };
    }
}

pub struct Color {
    a: u8,
    b: u8,
    g: u8,
    r: u8,
}

impl Color {
    pub fn r(&self) -> f64 { f64::from(self.r) / 255. }
    pub fn g(&self) -> f64 { f64::from(self.g) / 255. }
    pub fn b(&self) -> f64 { f64::from(self.b) / 255. }
    pub fn a(&self) -> f64 { f64::from(self.a) / 255. }
    pub fn rgba(&self) -> (f64, f64, f64, f64) { (self.r(), self.g(), self.b(), self.a()) }

    pub fn new(str: &str) -> Self {
        if str.len() == 0 {
            Self { r: 0, g: 0, b: 0, a: 0xff } // 黑不透明
        } else if str.chars().nth(0).unwrap() == '#' {
            fn char2value(c: char) -> u8 {
                let c = c as u8;
                if c >= b'0' && c <= b'9' {
                    c - b'0'
                } else if c >= b'A' && c <= b'F' {
                    c - b'A' + 0xA
                } else if c >= b'a' && c <= b'f' {
                    c - b'a' + 0xA
                } else {
                    0
                }
            }
            if str.len() == 4 || str.len() == 5 {
                let r = char2value(str.chars().nth(1).unwrap());
                let g = char2value(str.chars().nth(2).unwrap());
                let b = char2value(str.chars().nth(3).unwrap());
                if str.len() == 4 {
                    Self { r: r << 4 + r, g: g << 4 + g, b: b << 4 + b, a: 0xff }
                } else {
                    let a = char2value(str.chars().nth(4).unwrap());
                    Self { r: r << 4 + r, g: g << 4 + g, b: b << 4 + b, a: a << 4 + a }
                }
            } else if str.len() == 7 || str.len() == 9 {
                let r = char2value(str.chars().nth(1).unwrap()) << 4 + char2value(str.chars().nth(2).unwrap());
                let g = char2value(str.chars().nth(3).unwrap()) << 4 + char2value(str.chars().nth(4).unwrap());
                let b = char2value(str.chars().nth(5).unwrap()) << 4 + char2value(str.chars().nth(6).unwrap());
                if str.len() == 7 {
                    Self { r, g, b, a: 0xff }
                } else {
                    let a = char2value(str.chars().nth(7).unwrap()) << 4 + char2value(str.chars().nth(8).unwrap());
                    Self { r, g, b, a }
                }
            } else {
                Self { r: 0, g: 0, b: 0, a: 0xff } // 黑不透明
            }
        } else {
            let low_str = str.to_lowercase();
            match low_str.as_str() {
                "maroon" => Self::MAROON,
                "darkred" => Self::DARK_RED,
                "brown" => Self::BROWN,
                "firebrick" => Self::FIREBRICK,
                "crimson" => Self::CRIMSON,
                "red" => Self::RED,
                "tomato" => Self::TOMATO,
                "coral" => Self::CORAL,
                "indianred" => Self::INDIAN_RED,
                "lightcoral" => Self::LIGHT_CORAL,
                "darksalmon" => Self::DARK_SALMON,
                "salmon" => Self::SALMON,
                "lightsalmon" => Self::LIGHT_SALMON,
                "orangered" => Self::ORANGE_RED,
                "darkorange" => Self::DARK_ORANGE,
                "orange" => Self::ORANGE,
                "gold" => Self::GOLD,
                "darkgoldenrod" => Self::DARK_GOLDEN_ROD,
                "goldenrod" => Self::GOLDEN_ROD,
                "palegoldenrod" => Self::PALE_GOLDEN_ROD,
                "darkkhaki" => Self::DARK_KHAKI,
                "khaki" => Self::KHAKI,
                "olive" => Self::OLIVE,
                "yellow" => Self::YELLOW,
                "yellowgreen" => Self::YELLOW_GREEN,
                "darkolivegreen" => Self::DARK_OLIVE_GREEN,
                "olivedrab" => Self::OLIVE_DRAB,
                "lawngreen" => Self::LAWN_GREEN,
                "chartreuse" => Self::CHARTREUSE,
                "greenyellow" => Self::GREEN_YELLOW,
                "darkgreen" => Self::DARK_GREEN,
                "green" => Self::GREEN,
                "forestgreen" => Self::FOREST_GREEN,
                "lime" => Self::LIME,
                "limegreen" => Self::LIME_GREEN,
                "lightgreen" => Self::LIGHT_GREEN,
                "palegreen" => Self::PALE_GREEN,
                "darkseagreen" => Self::DARK_SEA_GREEN,
                "mediumspringgreen" => Self::MEDIUM_SPRING_GREEN,
                "springgreen" => Self::SPRING_GREEN,
                "seagreen" => Self::SEA_GREEN,
                "mediumaquamarine" => Self::MEDIUM_AQUA_MARINE,
                "mediumseagreen" => Self::MEDIUM_SEA_GREEN,
                "lightseagreen" => Self::LIGHT_SEA_GREEN,
                "darkslategray" => Self::DARK_SLATE_GRAY,
                "darkslategrey" => Self::DARK_SLATE_GREY,
                "teal" => Self::TEAL,
                "darkcyan" => Self::DARK_CYAN,
                "aqua" => Self::AQUA,
                "cyan" => Self::CYAN,
                "lightcyan" => Self::LIGHT_CYAN,
                "darkturquoise" => Self::DARK_TURQUOISE,
                "turquoise" => Self::TURQUOISE,
                "mediumturquoise" => Self::MEDIUM_TURQUOISE,
                "paleturquoise" => Self::PALE_TURQUOISE,
                "aquamarine" => Self::AQUA_MARINE,
                "powderblue" => Self::POWDER_BLUE,
                "cadetblue" => Self::CADET_BLUE,
                "steelblue" => Self::STEEL_BLUE,
                "cornflowerblue" => Self::CORN_FLOWER_BLUE,
                "deepskyblue" => Self::DEEP_SKY_BLUE,
                "dodgerblue" => Self::DODGER_BLUE,
                "lightblue" => Self::LIGHT_BLUE,
                "skyblue" => Self::SKY_BLUE,
                "lightskyblue" => Self::LIGHT_SKY_BLUE,
                "midnightblue" => Self::MIDNIGHT_BLUE,
                "navy" => Self::NAVY,
                "darkblue" => Self::DARK_BLUE,
                "mediumblue" => Self::MEDIUM_BLUE,
                "blue" => Self::BLUE,
                "royalblue" => Self::ROYAL_BLUE,
                "blueviolet" => Self::BLUE_VIOLET,
                "indigo" => Self::INDIGO,
                "darkslateblue" => Self::DARK_SLATE_BLUE,
                "slateblue" => Self::SLATE_BLUE,
                "mediumslateblue" => Self::MEDIUM_SLATE_BLUE,
                "mediumpurple" => Self::MEDIUM_PURPLE,
                "darkmagenta" => Self::DARK_MAGENTA,
                "darkviolet" => Self::DARK_VIOLET,
                "darkorchid" => Self::DARK_ORCHID,
                "mediumorchid" => Self::MEDIUM_ORCHID,
                "purple" => Self::PURPLE,
                "thistle" => Self::THISTLE,
                "plum" => Self::PLUM,
                "violet" => Self::VIOLET,
                "magenta" => Self::MAGENTA,
                "fuchsia" => Self::FUCHSIA,
                "orchid" => Self::ORCHID,
                "mediumvioletred" => Self::MEDIUM_VIOLET_RED,
                "palevioletred" => Self::PALE_VIOLET_RED,
                "deeppink" => Self::DEEP_PINK,
                "hotpink" => Self::HOT_PINK,
                "lightpink" => Self::LIGHT_PINK,
                "pink" => Self::PINK,
                "antiquewhite" => Self::ANTIQUE_WHITE,
                "beige" => Self::BEIGE,
                "bisque" => Self::BISQUE,
                "blanchedalmond" => Self::BLANCHED_ALMOND,
                "wheat" => Self::WHEAT,
                "cornsilk" => Self::CORN_SILK,
                "lemonchiffon" => Self::LEMON_CHIFFON,
                "lightgoldenrodyellow" => Self::LIGHT_GOLDEN_ROD_YELLOW,
                "lightyellow" => Self::LIGHT_YELLOW,
                "saddlebrown" => Self::SADDLE_BROWN,
                "sienna" => Self::SIENNA,
                "chocolate" => Self::CHOCOLATE,
                "peru" => Self::PERU,
                "sandybrown" => Self::SANDY_BROWN,
                "burlywood" => Self::BURLY_WOOD,
                "tan" => Self::TAN,
                "rosybrown" => Self::ROSY_BROWN,
                "moccasin" => Self::MOCCASIN,
                "navajowhite" => Self::NAVAJO_WHITE,
                "peachpuff" => Self::PEACH_PUFF,
                "mistyrose" => Self::MISTY_ROSE,
                "lavenderblush" => Self::LAVENDER_BLUSH,
                "linen" => Self::LINEN,
                "oldlace" => Self::OLD_LACE,
                "papayawhip" => Self::PAPAYA_WHIP,
                "seashell" => Self::SEA_SHELL,
                "mintcream" => Self::MINT_CREAM,
                "slategray" => Self::SLATE_GRAY,
                "slategrey" => Self::SLATE_GREY,
                "lightslategray" => Self::LIGHT_SLATE_GRAY,
                "lightslategrey" => Self::LIGHT_SLATE_GREY,
                "lightsteelblue" => Self::LIGHT_STEEL_BLUE,
                "lavender" => Self::LAVENDER,
                "floralwhite" => Self::FLORAL_WHITE,
                "aliceblue" => Self::ALICE_BLUE,
                "ghostwhite" => Self::GHOST_WHITE,
                "honeydew" => Self::HONEYDEW,
                "ivory" => Self::IVORY,
                "azure" => Self::AZURE,
                "snow" => Self::SNOW,
                "black" => Self::BLACK,
                "dimgray" => Self::DIM_GRAY,
                "dimgrey" => Self::DIM_GREY,
                "gray" => Self::GRAY,
                "grey" => Self::GREY,
                "darkgray" => Self::DARK_GRAY,
                "darkgrey" => Self::DARK_GREY,
                "silver" => Self::SILVER,
                "lightgray" => Self::LIGHT_GRAY,
                "lightgrey" => Self::LIGHT_GREY,
                "gainsboro" => Self::GAINSBORO,
                "whitesmoke" => Self::WHITE_SMOKE,
                "white" => Self::WHITE,
                "transparent" => Self::TRANSPARENT,
                _ => Self { r: 0, g: 0, b: 0, a: 0xff } // 黑不透明
            }
        }
    }

    // #000000FF ( 0,   0,   0,   255 ) Black
    // #FFFFFFFF ( 255, 255, 255, 255 ) White
    // #FF0000FF ( 255, 0,   0,   255 ) Red
    // #00FF00FF ( 0,   255, 0,   255 ) Lime
    // #0000FFFF ( 0,   0,   255, 255 ) Blue
    // #FFFF00FF ( 255, 255, 0,   255 ) Yellow
    // #00FFFFFF ( 0,   255, 255, 255 ) Aqua
    // #FF00FFFF ( 255, 0,   255, 255 ) Fuchsia
    // #C0C0C0FF ( 192, 192, 192, 255 ) Silver
    // #808080FF ( 128, 128, 128, 255 ) Gray
    // #800000FF ( 128, 0,   0,   255 ) Maroon
    // #808000FF ( 128, 128, 0,   255 ) Olive
    // #008000FF ( 0,   128, 0,   255 ) Green
    // #800080FF ( 128, 0,   128, 255 ) Purple
    // #008080FF ( 0,   128, 128, 255 ) Teal
    // #000080FF ( 0,   0,   128, 255 ) Navy

    pub const MAROON: Self = Self { r: 128, g: 0, b: 0, a: 255 };                       /* #800000FF ( 128, 0,   0,   255 ) Maroon               */
    pub const DARK_RED: Self = Self { r: 139, g: 0, b: 0, a: 255 };                     /* #8B0000FF ( 139, 0,   0,   255 ) DarkRed              */
    pub const BROWN: Self = Self { r: 165, g: 42, b: 42, a: 255 };                      /* #A52A2AFF ( 165, 42,  42,  255 ) Brown                */
    pub const FIREBRICK: Self = Self { r: 178, g: 34, b: 34, a: 255 };                  /* #B22222FF ( 178, 34,  34,  255 ) Firebrick            */
    pub const CRIMSON: Self = Self { r: 220, g: 20, b: 60, a: 255 };                    /* #DC143CFF ( 220, 20,  60,  255 ) Crimson              */
    pub const RED: Self = Self { r: 255, g: 0, b: 0, a: 255 };                          /* #FF0000FF ( 255, 0,   0,   255 ) Red                  */
    pub const TOMATO: Self = Self { r: 255, g: 99, b: 71, a: 255 };                     /* #FF6347FF ( 255, 99,  71,  255 ) Tomato               */
    pub const CORAL: Self = Self { r: 255, g: 127, b: 80, a: 255 };                     /* #FF7F50FF ( 255, 127, 80,  255 ) Coral                */
    pub const INDIAN_RED: Self = Self { r: 205, g: 92, b: 92, a: 255 };                 /* #CD5C5CFF ( 205, 92,  92,  255 ) IndianRed            */
    pub const LIGHT_CORAL: Self = Self { r: 240, g: 128, b: 128, a: 255 };              /* #F08080FF ( 240, 128, 128, 255 ) LightCoral           */
    pub const DARK_SALMON: Self = Self { r: 233, g: 150, b: 122, a: 255 };              /* #E9967AFF ( 233, 150, 122, 255 ) DarkSalmon           */
    pub const SALMON: Self = Self { r: 250, g: 128, b: 114, a: 255 };                   /* #FA8072FF ( 250, 128, 114, 255 ) Salmon               */
    pub const LIGHT_SALMON: Self = Self { r: 255, g: 160, b: 122, a: 255 };             /* #FFA07AFF ( 255, 160, 122, 255 ) LightSalmon          */
    pub const ORANGE_RED: Self = Self { r: 255, g: 69, b: 0, a: 255 };                  /* #FF4500FF ( 255, 69,  0,   255 ) OrangeRed            */
    pub const DARK_ORANGE: Self = Self { r: 255, g: 140, b: 0, a: 255 };                /* #FF8C00FF ( 255, 140, 0,   255 ) DarkOrange           */
    pub const ORANGE: Self = Self { r: 255, g: 165, b: 0, a: 255 };                     /* #FFA500FF ( 255, 165, 0,   255 ) Orange               */
    pub const GOLD: Self = Self { r: 255, g: 215, b: 0, a: 255 };                       /* #FFD700FF ( 255, 215, 0,   255 ) Gold                 */
    pub const DARK_GOLDEN_ROD: Self = Self { r: 184, g: 134, b: 11, a: 255 };           /* #B8860BFF ( 184, 134, 11,  255 ) DarkGoldenRod        */
    pub const GOLDEN_ROD: Self = Self { r: 218, g: 165, b: 32, a: 255 };                /* #DAA520FF ( 218, 165, 32,  255 ) GoldenRod            */
    pub const PALE_GOLDEN_ROD: Self = Self { r: 238, g: 232, b: 170, a: 255 };          /* #EEE8AAFF ( 238, 232, 170, 255 ) PaleGoldenRod        */
    pub const DARK_KHAKI: Self = Self { r: 189, g: 183, b: 107, a: 255 };               /* #BDB76BFF ( 189, 183, 107, 255 ) DarkKhaki            */
    pub const KHAKI: Self = Self { r: 240, g: 230, b: 140, a: 255 };                    /* #F0E68CFF ( 240, 230, 140, 255 ) Khaki                */
    pub const OLIVE: Self = Self { r: 128, g: 128, b: 0, a: 255 };                      /* #808000FF ( 128, 128, 0,   255 ) Olive                */
    pub const YELLOW: Self = Self { r: 255, g: 255, b: 0, a: 255 };                     /* #FFFF00FF ( 255, 255, 0,   255 ) Yellow               */
    pub const YELLOW_GREEN: Self = Self { r: 154, g: 205, b: 50, a: 255 };              /* #9ACD32FF ( 154, 205, 50,  255 ) YellowGreen          */
    pub const DARK_OLIVE_GREEN: Self = Self { r: 85, g: 107, b: 47, a: 255 };           /* #556B2FFF ( 85,  107, 47,  255 ) DarkOliveGreen       */
    pub const OLIVE_DRAB: Self = Self { r: 107, g: 142, b: 35, a: 255 };                /* #6B8E23FF ( 107, 142, 35,  255 ) OliveDrab            */
    pub const LAWN_GREEN: Self = Self { r: 124, g: 252, b: 0, a: 255 };                 /* #7CFC00FF ( 124, 252, 0,   255 ) LawnGreen            */
    pub const CHARTREUSE: Self = Self { r: 127, g: 255, b: 0, a: 255 };                 /* #7FFF00FF ( 127, 255, 0,   255 ) Chartreuse           */
    pub const GREEN_YELLOW: Self = Self { r: 173, g: 255, b: 47, a: 255 };              /* #ADFF2FFF ( 173, 255, 47,  255 ) GreenYellow          */
    pub const DARK_GREEN: Self = Self { r: 0, g: 100, b: 0, a: 255 };                   /* #006400FF ( 0,   100, 0,   255 ) DarkGreen            */
    pub const GREEN: Self = Self { r: 0, g: 128, b: 0, a: 255 };                        /* #008000FF ( 0,   128, 0,   255 ) Green                */
    pub const FOREST_GREEN: Self = Self { r: 34, g: 139, b: 34, a: 255 };               /* #228B22FF ( 34,  139, 34,  255 ) ForestGreen          */
    pub const LIME: Self = Self { r: 0, g: 255, b: 0, a: 255 };                         /* #00FF00FF ( 0,   255, 0,   255 ) Lime                 */
    pub const LIME_GREEN: Self = Self { r: 50, g: 205, b: 50, a: 255 };                 /* #32CD32FF ( 50,  205, 50,  255 ) LimeGreen            */
    pub const LIGHT_GREEN: Self = Self { r: 144, g: 238, b: 144, a: 255 };              /* #90EE90FF ( 144, 238, 144, 255 ) LightGreen           */
    pub const PALE_GREEN: Self = Self { r: 152, g: 251, b: 152, a: 255 };               /* #98FB98FF ( 152, 251, 152, 255 ) PaleGreen            */
    pub const DARK_SEA_GREEN: Self = Self { r: 143, g: 188, b: 143, a: 255 };           /* #8FBC8FFF ( 143, 188, 143, 255 ) DarkSeaGreen         */
    pub const MEDIUM_SPRING_GREEN: Self = Self { r: 0, g: 250, b: 154, a: 255 };        /* #00FA9AFF ( 0,   250, 154, 255 ) MediumSpringGreen    */
    pub const SPRING_GREEN: Self = Self { r: 0, g: 255, b: 127, a: 255 };               /* #00FF7FFF ( 0,   255, 127, 255 ) SpringGreen          */
    pub const SEA_GREEN: Self = Self { r: 46, g: 139, b: 87, a: 255 };                  /* #2E8B57FF ( 46,  139, 87,  255 ) SeaGreen             */
    pub const MEDIUM_AQUA_MARINE: Self = Self { r: 102, g: 205, b: 170, a: 255 };       /* #66CDAAFF ( 102, 205, 170, 255 ) MediumAquaMarine     */
    pub const MEDIUM_SEA_GREEN: Self = Self { r: 60, g: 179, b: 113, a: 255 };          /* #3CB371FF ( 60,  179, 113, 255 ) MediumSeaGreen       */
    pub const LIGHT_SEA_GREEN: Self = Self { r: 32, g: 178, b: 170, a: 255 };           /* #20B2AAFF ( 32,  178, 170, 255 ) LightSeaGreen        */
    pub const DARK_SLATE_GRAY: Self = Self { r: 47, g: 79, b: 79, a: 255 };             /* #2F4F4FFF ( 47,  79,  79,  255 ) DarkSlateGray        */
    pub const DARK_SLATE_GREY: Self = Self { r: 47, g: 79, b: 79, a: 255 };             /* #2F4F4FFF ( 47,  79,  79,  255 ) DarkSlateGrey        */
    pub const TEAL: Self = Self { r: 0, g: 128, b: 128, a: 255 };                       /* #008080FF ( 0,   128, 128, 255 ) Teal                 */
    pub const DARK_CYAN: Self = Self { r: 0, g: 139, b: 139, a: 255 };                  /* #008B8BFF ( 0,   139, 139, 255 ) DarkCyan             */
    pub const AQUA: Self = Self { r: 0, g: 255, b: 255, a: 255 };                       /* #00FFFFFF ( 0,   255, 255, 255 ) Aqua                 */
    pub const CYAN: Self = Self { r: 0, g: 255, b: 255, a: 255 };                       /* #00FFFFFF ( 0,   255, 255, 255 ) Cyan                 */
    pub const LIGHT_CYAN: Self = Self { r: 224, g: 255, b: 255, a: 255 };               /* #E0FFFFFF ( 224, 255, 255, 255 ) LightCyan            */
    pub const DARK_TURQUOISE: Self = Self { r: 0, g: 206, b: 209, a: 255 };             /* #00CED1FF ( 0,   206, 209, 255 ) DarkTurquoise        */
    pub const TURQUOISE: Self = Self { r: 64, g: 224, b: 208, a: 255 };                 /* #40E0D0FF ( 64,  224, 208, 255 ) Turquoise            */
    pub const MEDIUM_TURQUOISE: Self = Self { r: 72, g: 209, b: 204, a: 255 };          /* #48D1CCFF ( 72,  209, 204, 255 ) MediumTurquoise      */
    pub const PALE_TURQUOISE: Self = Self { r: 175, g: 238, b: 238, a: 255 };           /* #AFEEEEFF ( 175, 238, 238, 255 ) PaleTurquoise        */
    pub const AQUA_MARINE: Self = Self { r: 127, g: 255, b: 212, a: 255 };              /* #7FFFD4FF ( 127, 255, 212, 255 ) AquaMarine           */
    pub const POWDER_BLUE: Self = Self { r: 176, g: 224, b: 230, a: 255 };              /* #B0E0E6FF ( 176, 224, 230, 255 ) PowderBlue           */
    pub const CADET_BLUE: Self = Self { r: 95, g: 158, b: 160, a: 255 };                /* #5F9EA0FF ( 95,  158, 160, 255 ) CadetBlue            */
    pub const STEEL_BLUE: Self = Self { r: 70, g: 130, b: 180, a: 255 };                /* #4682B4FF ( 70,  130, 180, 255 ) SteelBlue            */
    pub const CORN_FLOWER_BLUE: Self = Self { r: 100, g: 149, b: 237, a: 255 };         /* #6495EDFF ( 100, 149, 237, 255 ) CornFlowerBlue       */
    pub const DEEP_SKY_BLUE: Self = Self { r: 0, g: 191, b: 255, a: 255 };              /* #00BFFFFF ( 0,   191, 255, 255 ) DeepSkyBlue          */
    pub const DODGER_BLUE: Self = Self { r: 30, g: 144, b: 255, a: 255 };               /* #1E90FFFF ( 30,  144, 255, 255 ) DodgerBlue           */
    pub const LIGHT_BLUE: Self = Self { r: 173, g: 216, b: 230, a: 255 };               /* #ADD8E6FF ( 173, 216, 230, 255 ) LightBlue            */
    pub const SKY_BLUE: Self = Self { r: 135, g: 206, b: 235, a: 255 };                 /* #87CEEBFF ( 135, 206, 235, 255 ) SkyBlue              */
    pub const LIGHT_SKY_BLUE: Self = Self { r: 135, g: 206, b: 250, a: 255 };           /* #87CEFAFF ( 135, 206, 250, 255 ) LightSkyBlue         */
    pub const MIDNIGHT_BLUE: Self = Self { r: 25, g: 25, b: 112, a: 255 };              /* #191970FF ( 25,  25,  112, 255 ) MidnightBlue         */
    pub const NAVY: Self = Self { r: 0, g: 0, b: 128, a: 255 };                         /* #000080FF ( 0,   0,   128, 255 ) Navy                 */
    pub const DARK_BLUE: Self = Self { r: 0, g: 0, b: 139, a: 255 };                    /* #00008BFF ( 0,   0,   139, 255 ) DarkBlue             */
    pub const MEDIUM_BLUE: Self = Self { r: 0, g: 0, b: 205, a: 255 };                  /* #0000CDFF ( 0,   0,   205, 255 ) MediumBlue           */
    pub const BLUE: Self = Self { r: 0, g: 0, b: 255, a: 255 };                         /* #0000FFFF ( 0,   0,   255, 255 ) Blue                 */
    pub const ROYAL_BLUE: Self = Self { r: 65, g: 105, b: 225, a: 255 };                /* #4169E1FF ( 65,  105, 225, 255 ) RoyalBlue            */
    pub const BLUE_VIOLET: Self = Self { r: 138, g: 43, b: 226, a: 255 };               /* #8A2BE2FF ( 138, 43,  226, 255 ) BlueViolet           */
    pub const INDIGO: Self = Self { r: 75, g: 0, b: 130, a: 255 };                      /* #4B0082FF ( 75,  0,   130, 255 ) Indigo               */
    pub const DARK_SLATE_BLUE: Self = Self { r: 72, g: 61, b: 139, a: 255 };            /* #483D8BFF ( 72,  61,  139, 255 ) DarkSlateBlue        */
    pub const SLATE_BLUE: Self = Self { r: 106, g: 90, b: 205, a: 255 };                /* #6A5ACDFF ( 106, 90,  205, 255 ) SlateBlue            */
    pub const MEDIUM_SLATE_BLUE: Self = Self { r: 123, g: 104, b: 238, a: 255 };        /* #7B68EEFF ( 123, 104, 238, 255 ) MediumSlateBlue      */
    pub const MEDIUM_PURPLE: Self = Self { r: 147, g: 112, b: 219, a: 255 };            /* #9370DBFF ( 147, 112, 219, 255 ) MediumPurple         */
    pub const DARK_MAGENTA: Self = Self { r: 139, g: 0, b: 139, a: 255 };               /* #8B008BFF ( 139, 0,   139, 255 ) DarkMagenta          */
    pub const DARK_VIOLET: Self = Self { r: 148, g: 0, b: 211, a: 255 };                /* #9400D3FF ( 148, 0,   211, 255 ) DarkViolet           */
    pub const DARK_ORCHID: Self = Self { r: 153, g: 50, b: 204, a: 255 };               /* #9932CCFF ( 153, 50,  204, 255 ) DarkOrchid           */
    pub const MEDIUM_ORCHID: Self = Self { r: 186, g: 85, b: 211, a: 255 };             /* #BA55D3FF ( 186, 85,  211, 255 ) MediumOrchid         */
    pub const PURPLE: Self = Self { r: 128, g: 0, b: 128, a: 255 };                     /* #800080FF ( 128, 0,   128, 255 ) Purple               */
    pub const THISTLE: Self = Self { r: 216, g: 191, b: 216, a: 255 };                  /* #D8BFD8FF ( 216, 191, 216, 255 ) Thistle              */
    pub const PLUM: Self = Self { r: 221, g: 160, b: 221, a: 255 };                     /* #DDA0DDFF ( 221, 160, 221, 255 ) Plum                 */
    pub const VIOLET: Self = Self { r: 238, g: 130, b: 238, a: 255 };                   /* #EE82EEFF ( 238, 130, 238, 255 ) Violet               */
    pub const MAGENTA: Self = Self { r: 255, g: 0, b: 255, a: 255 };                    /* #FF00FFFF ( 255, 0,   255, 255 ) Magenta              */
    pub const FUCHSIA: Self = Self { r: 255, g: 0, b: 255, a: 255 };                    /* #FF00FFFF ( 255, 0,   255, 255 ) Fuchsia              */
    pub const ORCHID: Self = Self { r: 218, g: 112, b: 214, a: 255 };                   /* #DA70D6FF ( 218, 112, 214, 255 ) Orchid               */
    pub const MEDIUM_VIOLET_RED: Self = Self { r: 199, g: 21, b: 133, a: 255 };         /* #C71585FF ( 199, 21,  133, 255 ) MediumVioletRed      */
    pub const PALE_VIOLET_RED: Self = Self { r: 219, g: 112, b: 147, a: 255 };          /* #DB7093FF ( 219, 112, 147, 255 ) PaleVioletRed        */
    pub const DEEP_PINK: Self = Self { r: 255, g: 20, b: 147, a: 255 };                 /* #FF1493FF ( 255, 20,  147, 255 ) DeepPink             */
    pub const HOT_PINK: Self = Self { r: 255, g: 105, b: 180, a: 255 };                 /* #FF69B4FF ( 255, 105, 180, 255 ) HotPink              */
    pub const LIGHT_PINK: Self = Self { r: 255, g: 182, b: 193, a: 255 };               /* #FFB6C1FF ( 255, 182, 193, 255 ) LightPink            */
    pub const PINK: Self = Self { r: 255, g: 192, b: 203, a: 255 };                     /* #FFC0CBFF ( 255, 192, 203, 255 ) Pink                 */
    pub const ANTIQUE_WHITE: Self = Self { r: 250, g: 235, b: 215, a: 255 };            /* #FAEBD7FF ( 250, 235, 215, 255 ) AntiqueWhite         */
    pub const BEIGE: Self = Self { r: 245, g: 245, b: 220, a: 255 };                    /* #F5F5DCFF ( 245, 245, 220, 255 ) Beige                */
    pub const BISQUE: Self = Self { r: 255, g: 228, b: 196, a: 255 };                   /* #FFE4C4FF ( 255, 228, 196, 255 ) Bisque               */
    pub const BLANCHED_ALMOND: Self = Self { r: 255, g: 235, b: 205, a: 255 };          /* #FFEBCDFF ( 255, 235, 205, 255 ) BlanchedAlmond       */
    pub const WHEAT: Self = Self { r: 245, g: 222, b: 179, a: 255 };                    /* #F5DEB3FF ( 245, 222, 179, 255 ) Wheat                */
    pub const CORN_SILK: Self = Self { r: 255, g: 248, b: 220, a: 255 };                /* #FFF8DCFF ( 255, 248, 220, 255 ) CornSilk             */
    pub const LEMON_CHIFFON: Self = Self { r: 255, g: 250, b: 205, a: 255 };            /* #FFFACDFF ( 255, 250, 205, 255 ) LemonChiffon         */
    pub const LIGHT_GOLDEN_ROD_YELLOW: Self = Self { r: 250, g: 250, b: 210, a: 255 };  /* #FAFAD2FF ( 250, 250, 210, 255 ) LightGoldenRodYellow */
    pub const LIGHT_YELLOW: Self = Self { r: 255, g: 255, b: 224, a: 255 };             /* #FFFFE0FF ( 255, 255, 224, 255 ) LightYellow          */
    pub const SADDLE_BROWN: Self = Self { r: 139, g: 69, b: 19, a: 255 };               /* #8B4513FF ( 139, 69,  19,  255 ) SaddleBrown          */
    pub const SIENNA: Self = Self { r: 160, g: 82, b: 45, a: 255 };                     /* #A0522DFF ( 160, 82,  45,  255 ) Sienna               */
    pub const CHOCOLATE: Self = Self { r: 210, g: 105, b: 30, a: 255 };                 /* #D2691EFF ( 210, 105, 30,  255 ) Chocolate            */
    pub const PERU: Self = Self { r: 205, g: 133, b: 63, a: 255 };                      /* #CD853FFF ( 205, 133, 63,  255 ) Peru                 */
    pub const SANDY_BROWN: Self = Self { r: 244, g: 164, b: 96, a: 255 };               /* #F4A460FF ( 244, 164, 96,  255 ) SandyBrown           */
    pub const BURLY_WOOD: Self = Self { r: 222, g: 184, b: 135, a: 255 };               /* #DEB887FF ( 222, 184, 135, 255 ) BurlyWood            */
    pub const TAN: Self = Self { r: 210, g: 180, b: 140, a: 255 };                      /* #D2B48CFF ( 210, 180, 140, 255 ) Tan                  */
    pub const ROSY_BROWN: Self = Self { r: 188, g: 143, b: 143, a: 255 };               /* #BC8F8FFF ( 188, 143, 143, 255 ) RosyBrown            */
    pub const MOCCASIN: Self = Self { r: 255, g: 228, b: 181, a: 255 };                 /* #FFE4B5FF ( 255, 228, 181, 255 ) Moccasin             */
    pub const NAVAJO_WHITE: Self = Self { r: 255, g: 222, b: 173, a: 255 };             /* #FFDEADFF ( 255, 222, 173, 255 ) NavajoWhite          */
    pub const PEACH_PUFF: Self = Self { r: 255, g: 218, b: 185, a: 255 };               /* #FFDAB9FF ( 255, 218, 185, 255 ) PeachPuff            */
    pub const MISTY_ROSE: Self = Self { r: 255, g: 228, b: 225, a: 255 };               /* #FFE4E1FF ( 255, 228, 225, 255 ) MistyRose            */
    pub const LAVENDER_BLUSH: Self = Self { r: 255, g: 240, b: 245, a: 255 };           /* #FFF0F5FF ( 255, 240, 245, 255 ) LavenderBlush        */
    pub const LINEN: Self = Self { r: 250, g: 240, b: 230, a: 255 };                    /* #FAF0E6FF ( 250, 240, 230, 255 ) Linen                */
    pub const OLD_LACE: Self = Self { r: 253, g: 245, b: 230, a: 255 };                 /* #FDF5E6FF ( 253, 245, 230, 255 ) OldLace              */
    pub const PAPAYA_WHIP: Self = Self { r: 255, g: 239, b: 213, a: 255 };              /* #FFEFD5FF ( 255, 239, 213, 255 ) PapayaWhip           */
    pub const SEA_SHELL: Self = Self { r: 255, g: 245, b: 238, a: 255 };                /* #FFF5EEFF ( 255, 245, 238, 255 ) SeaShell             */
    pub const MINT_CREAM: Self = Self { r: 245, g: 255, b: 250, a: 255 };               /* #F5FFFAFF ( 245, 255, 250, 255 ) MintCream            */
    pub const SLATE_GRAY: Self = Self { r: 112, g: 128, b: 144, a: 255 };               /* #708090FF ( 112, 128, 144, 255 ) SlateGray            */
    pub const SLATE_GREY: Self = Self { r: 112, g: 128, b: 144, a: 255 };               /* #708090FF ( 112, 128, 144, 255 ) SlateGrey            */
    pub const LIGHT_SLATE_GRAY: Self = Self { r: 119, g: 136, b: 153, a: 255 };         /* #778899FF ( 119, 136, 153, 255 ) LightSlateGray       */
    pub const LIGHT_SLATE_GREY: Self = Self { r: 119, g: 136, b: 153, a: 255 };         /* #778899FF ( 119, 136, 153, 255 ) LightSlateGrey       */
    pub const LIGHT_STEEL_BLUE: Self = Self { r: 176, g: 196, b: 222, a: 255 };         /* #B0C4DEFF ( 176, 196, 222, 255 ) LightSteelBlue       */
    pub const LAVENDER: Self = Self { r: 230, g: 230, b: 250, a: 255 };                 /* #E6E6FAFF ( 230, 230, 250, 255 ) Lavender             */
    pub const FLORAL_WHITE: Self = Self { r: 255, g: 250, b: 240, a: 255 };             /* #FFFAF0FF ( 255, 250, 240, 255 ) FloralWhite          */
    pub const ALICE_BLUE: Self = Self { r: 240, g: 248, b: 255, a: 255 };               /* #F0F8FFFF ( 240, 248, 255, 255 ) AliceBlue            */
    pub const GHOST_WHITE: Self = Self { r: 248, g: 248, b: 255, a: 255 };              /* #F8F8FFFF ( 248, 248, 255, 255 ) GhostWhite           */
    pub const HONEYDEW: Self = Self { r: 240, g: 255, b: 240, a: 255 };                 /* #F0FFF0FF ( 240, 255, 240, 255 ) Honeydew             */
    pub const IVORY: Self = Self { r: 255, g: 255, b: 240, a: 255 };                    /* #FFFFF0FF ( 255, 255, 240, 255 ) Ivory                */
    pub const AZURE: Self = Self { r: 240, g: 255, b: 255, a: 255 };                    /* #F0FFFFFF ( 240, 255, 255, 255 ) Azure                */
    pub const SNOW: Self = Self { r: 255, g: 250, b: 250, a: 255 };                     /* #FFFAFAFF ( 255, 250, 250, 255 ) Snow                 */
    pub const BLACK: Self = Self { r: 0, g: 0, b: 0, a: 255 };                          /* #000000FF ( 0,   0,   0,   255 ) Black                */
    pub const DIM_GRAY: Self = Self { r: 105, g: 105, b: 105, a: 255 };                 /* #696969FF ( 105, 105, 105, 255 ) DimGray              */
    pub const DIM_GREY: Self = Self { r: 105, g: 105, b: 105, a: 255 };                 /* #696969FF ( 105, 105, 105, 255 ) DimGrey              */
    pub const GRAY: Self = Self { r: 128, g: 128, b: 128, a: 255 };                     /* #808080FF ( 128, 128, 128, 255 ) Gray                 */
    pub const GREY: Self = Self { r: 128, g: 128, b: 128, a: 255 };                     /* #808080FF ( 128, 128, 128, 255 ) Grey                 */
    pub const DARK_GRAY: Self = Self { r: 169, g: 169, b: 169, a: 255 };                /* #A9A9A9FF ( 169, 169, 169, 255 ) DarkGray             */
    pub const DARK_GREY: Self = Self { r: 169, g: 169, b: 169, a: 255 };                /* #A9A9A9FF ( 169, 169, 169, 255 ) DarkGrey             */
    pub const SILVER: Self = Self { r: 192, g: 192, b: 192, a: 255 };                   /* #C0C0C0FF ( 192, 192, 192, 255 ) Silver               */
    pub const LIGHT_GRAY: Self = Self { r: 211, g: 211, b: 211, a: 255 };               /* #D3D3D3FF ( 211, 211, 211, 255 ) LightGray            */
    pub const LIGHT_GREY: Self = Self { r: 211, g: 211, b: 211, a: 255 };               /* #D3D3D3FF ( 211, 211, 211, 255 ) LightGrey            */
    pub const GAINSBORO: Self = Self { r: 220, g: 220, b: 220, a: 255 };                /* #DCDCDCFF ( 220, 220, 220, 255 ) Gainsboro            */
    pub const WHITE_SMOKE: Self = Self { r: 245, g: 245, b: 245, a: 255 };              /* #F5F5F5FF ( 245, 245, 245, 255 ) WhiteSmoke           */
    pub const WHITE: Self = Self { r: 255, g: 255, b: 255, a: 255 };                    /* #FFFFFFFF ( 255, 255, 255, 255 ) White                */
    pub const TRANSPARENT: Self = Self { r: 0, g: 0, b: 0, a: 0 };                      /* #00000000 ( 0,   0,   0,   0   ) Transparent          */
}

pub enum HorizontalAlign {
    Left,
    Center,
    Right,
}

pub enum VerticalAlign {
    BaseLine,
    Top,
    Center,
    Bottom,
}
