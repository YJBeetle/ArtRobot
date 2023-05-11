use std::f64::consts::PI;
use cairo::{RecordingSurface, Context};
use pangocairo::pango;
use pangocairo::pango::{Alignment, EllipsizeMode, FontDescription, Weight};
use crate::types::{Color, HorizontalAlign, Property, Transform, VerticalAlign};

struct Base {
    surface: RecordingSurface,
    cr: Context,
    property: Property,
    transform: Transform,
}

impl Base {
    fn new(property: Option<Property>, transform: Option<Transform>) -> Self {
        let surface = RecordingSurface::create(cairo::Content::Alpha, None).unwrap();
        let cr = Context::new(&surface).unwrap();
        let property = property.unwrap_or_default();
        let transform = transform.unwrap_or_default();
        let ret = Self {
            surface,
            cr,
            property,
            transform,
        };
        ret.transform.apply(&ret.property, &ret.cr);
        ret
    }

    fn surface(&self) -> &RecordingSurface { &self.surface }
}


pub struct Rectangle {
    base: Base,
}

impl Rectangle {
    pub fn new(name: String, transform: Transform,
               width: f64, height: f64,
               color: Color) -> Self {
        let base = Base::new(Some(Property { name, width, height }), Some(transform));
        base.cr.set_source_rgba(color.r(), color.g(), color.b(), color.a());
        base.cr.rectangle(0., 0., width, height);
        base.cr.fill().unwrap();
        Self { base }
    }
    pub fn surface(&self) -> &RecordingSurface { &self.base.surface() }
}


pub struct RectangleRound {
    base: Base,
}

impl RectangleRound {
    pub fn new(name: String, transform: Transform,
               width: f64, height: f64,
               angle_tl: f64, angle_tr: f64, angle_br: f64, angle_bl: f64,
               color: Color) -> Self {
        let base = Base::new(Some(Property { name, width, height }), Some(transform));

        base.cr.set_source_rgba(color.r(), color.g(), color.b(), color.a());

        base.cr.move_to(angle_tl, 0.);
        base.cr.line_to(width - angle_tl, 0.);

        base.cr.move_to(width, angle_tr);
        base.cr.line_to(width, height - angle_tr);

        base.cr.move_to(width - angle_br, height);
        base.cr.line_to(angle_br, height);

        base.cr.move_to(0., height - angle_bl);
        base.cr.line_to(0., angle_bl);

        base.cr.arc(angle_tl, angle_tl, angle_tl, PI, 3. * PI / 2.0);
        base.cr.arc(width - angle_tr, angle_tr, angle_tr, 3. * PI / 2., 2. * PI);
        base.cr.arc(width - angle_br, height - angle_br, angle_br, 0., PI / 2.);
        base.cr.arc(angle_bl, height - angle_bl, angle_bl, PI / 2., PI);

        base.cr.fill().unwrap();

        Self { base }
    }
    pub fn surface(&self) -> &RecordingSurface { &self.base.surface() }
}


pub struct Circle {
    base: Base,
}

impl Circle {
    pub fn new(name: String, transform: Transform,
               width: f64, height: f64,
               color: Color) -> Self {
        let base = Base::new(Some(Property { name, width, height }), Some(transform));
        base.cr.set_source_rgba(color.r(), color.g(), color.b(), color.a());
        base.cr.scale(width / 2., height / 2.);
        base.cr.arc(1., 1., 1., 0., 2. * PI);
        base.cr.fill().unwrap();
        Self { base }
    }
    pub fn surface(&self) -> &RecordingSurface { &self.base.surface() }
}


pub struct Svg {}


pub struct Image {}


pub struct Mask {}


pub struct Text {
    base: Base,
}

impl Text {
    pub fn new(name: String, transform: Transform,
               content: String,                  // 内容
               color: Color,                     // 颜色
               font_family: Option<String>,      // 字体
               font_weight: Option<i32>,         // 粗细
               font_size: Option<f64>,           // 字号
               h_align: Option<HorizontalAlign>, // 水平对齐方式
               v_align: Option<VerticalAlign>,   // 垂直对齐方式
               max_width: Option<f64>,           // 最大宽度
               line_spacing: Option<f64>,        // 行间距
               _word_spacing: Option<f64>,       // 字间距
    ) -> Self {
        let font_family = font_family.unwrap_or(String::new());
        let font_weight = font_weight.unwrap_or(500);
        let font_size = font_size.unwrap_or(10.);
        let h_align = h_align.unwrap_or(HorizontalAlign::Center);
        let v_align = v_align.unwrap_or(VerticalAlign::BaseLine);
        let max_width = max_width.unwrap_or(f64::NAN);
        let line_spacing = line_spacing.unwrap_or(0.);
        let _word_spacing = _word_spacing.unwrap_or(0.);

        let mut base = Base::new(Some(Property { name, width: f64::NAN, height: f64::NAN }), Some(transform));
        base.cr.set_source_rgba(color.r(), color.g(), color.b(), color.a());

        let layout = pangocairo::create_layout(&base.cr);
        layout.set_text(content.as_str());
        let mut desc = FontDescription::new();
        desc.set_family(font_family.as_str());
        desc.set_weight(match font_weight {
            0..=100 => Weight::Thin,
            101..=200 => Weight::Ultralight,
            201..=300 => Weight::Light,
            301..=350 => Weight::Semilight,
            351..=380 => Weight::Book,
            381..=400 => Weight::Normal,
            401..=500 => Weight::Medium,
            501..=600 => Weight::Semibold,
            601..=700 => Weight::Bold,
            701..=800 => Weight::Ultrabold,
            801..=900 => Weight::Heavy,
            901..=1000 => Weight::Ultraheavy,
            _ => Weight::Normal,
        });
        desc.set_size((font_size * pango::SCALE as f64) as i32);
        layout.set_font_description(Some(&desc));

        if !max_width.is_nan() {
            layout.set_width((max_width * pango::SCALE as f64) as i32); // 设置界定框
            layout.set_ellipsize(EllipsizeMode::End); // 超出用省略号
        }

        layout.set_spacing((line_spacing * pango::SCALE as f64) as i32); // 行距

        // 水平对齐
        match h_align {
            HorizontalAlign::Left => layout.set_alignment(Alignment::Left),
            HorizontalAlign::Center => layout.set_alignment(Alignment::Center),
            HorizontalAlign::Right => layout.set_alignment(Alignment::Right),
        }

        pangocairo::update_layout(&base.cr, &layout);
        let (layout_width, layout_height) = layout.size();
        let real_w = layout_width as f64 / pango::SCALE as f64;
        let real_h = layout_height as f64 / pango::SCALE as f64;

        // 水平对齐
        let x_move =
            match h_align {
                HorizontalAlign::Left => 0.,
                HorizontalAlign::Center => -real_w / 2.,
                HorizontalAlign::Right => -real_w,
            };
        // 垂直对齐
        let y_move =
            match v_align {
                VerticalAlign::BaseLine =>
                    -layout.baseline() as f64 / pango::SCALE as f64,
                VerticalAlign::Top =>
                    0.,
                VerticalAlign::Center =>
                    -real_h / 2.,
                VerticalAlign::Bottom =>
                    -real_h,
            };
        // 移动
        base.cr.move_to(x_move, y_move);

        pangocairo::show_layout(&base.cr, &layout);

        base.property.width = real_w;
        base.property.height = real_h;

        Self { base }
    }
    pub fn surface(&self) -> &RecordingSurface { &self.base.surface() }
}


pub struct TextArea {}


pub struct Repeat {}


pub struct Group {}
