use std::f64::consts::PI;
use std::path::Path;

use cairo::{Context, Extend, Format, ImageSurface, RecordingSurface};
use pangocairo::pango;
use pangocairo::pango::{Alignment, EllipsizeMode, FontDescription, Weight, WrapMode};

use crate::error::{Error, Result};
use crate::types::{Color, HorizontalAlign, Property, Transform, VerticalAlign};

struct ComponentBase {
    surface: RecordingSurface,
    cr: Context,
    property: Property,
}

impl ComponentBase {
    fn new(property: Option<Property>, transform: Option<Transform>) -> Self {
        let surface = RecordingSurface::create(cairo::Content::ColorAlpha, None).unwrap();
        let cr = Context::new(&surface).unwrap();
        let property = property.unwrap_or_default();
        let transform = transform.unwrap_or_default();
        let ret = Self {
            surface,
            cr,
            property,
        };
        transform.apply(&ret.property, &ret.cr);
        ret
    }

    fn surface(&self) -> &RecordingSurface {
        &self.surface
    }

    fn property(&self) -> &Property {
        &self.property
    }
}

pub trait Component {
    fn surface(&self) -> &RecordingSurface;
    fn property(&self) -> &Property;

    fn width(&self) -> f64 {
        self.property().width
    }

    fn height(&self) -> f64 {
        self.property().height
    }
}

impl<T: Component + ?Sized> Component for &T {
    fn surface(&self) -> &RecordingSurface {
        (*self).surface()
    }

    fn property(&self) -> &Property {
        (*self).property()
    }
}

pub struct Rectangle {
    base: ComponentBase,
}

impl Rectangle {
    pub fn new(name: String, transform: Transform, width: f64, height: f64, color: Color) -> Self {
        let base = ComponentBase::new(
            Some(Property {
                name,
                width,
                height,
            }),
            Some(transform),
        );
        base.cr
            .set_source_rgba(color.r(), color.g(), color.b(), color.a());
        base.cr.rectangle(0., 0., width, height);
        base.cr.fill().unwrap();
        Self { base }
    }
}

impl Component for Rectangle {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

pub struct RectangleRound {
    base: ComponentBase,
}

impl RectangleRound {
    #[allow(clippy::too_many_arguments)]
    pub fn new(
        name: String,
        transform: Transform,
        width: f64,
        height: f64,
        angle_tl: f64,
        angle_tr: f64,
        angle_br: f64,
        angle_bl: f64,
        color: Color,
    ) -> Self {
        let base = ComponentBase::new(
            Some(Property {
                name,
                width,
                height,
            }),
            Some(transform),
        );

        base.cr
            .set_source_rgba(color.r(), color.g(), color.b(), color.a());

        base.cr.move_to(angle_tl, 0.);
        base.cr.line_to(width - angle_tl, 0.);

        base.cr.move_to(width, angle_tr);
        base.cr.line_to(width, height - angle_tr);

        base.cr.move_to(width - angle_br, height);
        base.cr.line_to(angle_br, height);

        base.cr.move_to(0., height - angle_bl);
        base.cr.line_to(0., angle_bl);

        base.cr.arc(angle_tl, angle_tl, angle_tl, PI, 3. * PI / 2.0);
        base.cr
            .arc(width - angle_tr, angle_tr, angle_tr, 3. * PI / 2., 2. * PI);
        base.cr
            .arc(width - angle_br, height - angle_br, angle_br, 0., PI / 2.);
        base.cr
            .arc(angle_bl, height - angle_bl, angle_bl, PI / 2., PI);

        base.cr.fill().unwrap();

        Self { base }
    }
}

impl Component for RectangleRound {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

pub struct Circle {
    base: ComponentBase,
}

impl Circle {
    pub fn new(name: String, transform: Transform, width: f64, height: f64, color: Color) -> Self {
        let base = ComponentBase::new(
            Some(Property {
                name,
                width,
                height,
            }),
            Some(transform),
        );
        base.cr
            .set_source_rgba(color.r(), color.g(), color.b(), color.a());
        base.cr.scale(width / 2., height / 2.);
        base.cr.arc(1., 1., 1., 0., 2. * PI);
        base.cr.fill().unwrap();
        Self { base }
    }
}

impl Component for Circle {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

fn target_dimensions(
    source_width: f64,
    source_height: f64,
    width: Option<f64>,
    height: Option<f64>,
) -> Result<(f64, f64)> {
    if source_width <= 0. || source_height <= 0. {
        return Err(Error::InvalidInput(
            "image dimensions must be greater than zero".into(),
        ));
    }
    if width.is_some_and(|value| !value.is_finite() || value <= 0.)
        || height.is_some_and(|value| !value.is_finite() || value <= 0.)
    {
        return Err(Error::InvalidInput(
            "target dimensions must be finite and greater than zero".into(),
        ));
    }

    Ok(match (width, height) {
        (Some(width), Some(height)) => (width, height),
        (Some(width), None) => (width, width * source_height / source_width),
        (None, Some(height)) => (height * source_width / source_height, height),
        (None, None) => (source_width, source_height),
    })
}

fn paint_image_surface(
    name: String,
    transform: Transform,
    surface: &ImageSurface,
    source_width: f64,
    source_height: f64,
    width: Option<f64>,
    height: Option<f64>,
) -> Result<ComponentBase> {
    let (width, height) = target_dimensions(source_width, source_height, width, height)?;
    let base = ComponentBase::new(
        Some(Property {
            name,
            width,
            height,
        }),
        Some(transform),
    );
    base.cr.save()?;
    base.cr.scale(width / source_width, height / source_height);
    base.cr.set_source_surface(surface, 0., 0.)?;
    base.cr.source().set_extend(Extend::Pad);
    base.cr.paint()?;
    base.cr.restore()?;
    Ok(base)
}

fn surface_from_rgba(
    width: u32,
    height: u32,
    rgba: &[u8],
    premultiplied: bool,
) -> Result<ImageSurface> {
    if width == 0 || height == 0 || rgba.len() != width as usize * height as usize * 4 {
        return Err(Error::InvalidInput("invalid RGBA image buffer".into()));
    }
    let stride = i32::try_from(
        width
            .checked_mul(4)
            .ok_or_else(|| Error::InvalidInput("image row is too large".into()))?,
    )
    .map_err(|_| Error::InvalidInput("image row is too large".into()))?;
    let mut argb = Vec::with_capacity(rgba.len());
    for pixel in rgba.chunks_exact(4) {
        let alpha = pixel[3];
        let (red, green, blue) = if premultiplied {
            (pixel[0], pixel[1], pixel[2])
        } else {
            (
                (u16::from(pixel[0]) * u16::from(alpha) / 255) as u8,
                (u16::from(pixel[1]) * u16::from(alpha) / 255) as u8,
                (u16::from(pixel[2]) * u16::from(alpha) / 255) as u8,
            )
        };
        argb.extend_from_slice(
            &(u32::from(alpha) << 24
                | u32::from(red) << 16
                | u32::from(green) << 8
                | u32::from(blue))
            .to_ne_bytes(),
        );
    }
    Ok(ImageSurface::create_for_data(
        argb,
        Format::ARgb32,
        width as i32,
        height as i32,
        stride,
    )?)
}

/// A decoded PNG, JPEG, WebP, or native Cairo ARGB32 image component.
pub struct Image {
    base: ComponentBase,
}

impl Image {
    pub fn from_bytes(
        name: String,
        transform: Transform,
        data: &[u8],
        width: Option<f64>,
        height: Option<f64>,
    ) -> Result<Self> {
        let decoded = image::load_from_memory(data)?.into_rgba8();
        let source_width = decoded.width();
        let source_height = decoded.height();
        let surface = surface_from_rgba(source_width, source_height, decoded.as_raw(), false)?;
        let base = paint_image_surface(
            name,
            transform,
            &surface,
            f64::from(source_width),
            f64::from(source_height),
            width,
            height,
        )?;
        Ok(Self { base })
    }

    pub fn from_file(
        name: String,
        transform: Transform,
        path: impl AsRef<Path>,
        width: Option<f64>,
        height: Option<f64>,
    ) -> Result<Self> {
        Self::from_bytes(name, transform, &std::fs::read(path)?, width, height)
    }

    /// Creates an image from BGRA bytes. The input is copied, so callers may reuse it immediately.
    #[allow(clippy::too_many_arguments)]
    pub fn from_bgra(
        name: String,
        transform: Transform,
        data: &[u8],
        image_width: u32,
        image_height: u32,
        stride: usize,
        premultiplied: bool,
        width: Option<f64>,
        height: Option<f64>,
    ) -> Result<Self> {
        let row_bytes = image_width as usize * 4;
        let required = stride
            .checked_mul(image_height as usize)
            .ok_or_else(|| Error::InvalidInput("raw image buffer is too large".into()))?;
        if image_width == 0 || image_height == 0 || stride < row_bytes || data.len() < required {
            return Err(Error::InvalidInput("invalid raw BGRA image buffer".into()));
        }

        let mut rgba = Vec::with_capacity(row_bytes * image_height as usize);
        for row in data.chunks(stride).take(image_height as usize) {
            for pixel in row[..row_bytes].chunks_exact(4) {
                rgba.extend_from_slice(&[pixel[2], pixel[1], pixel[0], pixel[3]]);
            }
        }
        let surface = surface_from_rgba(image_width, image_height, &rgba, premultiplied)?;
        let base = paint_image_surface(
            name,
            transform,
            &surface,
            f64::from(image_width),
            f64::from(image_height),
            width,
            height,
        )?;
        Ok(Self { base })
    }
}

impl Component for Image {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

/// An SVG component rasterized with resvg before being recorded by Cairo.
pub struct Svg {
    base: ComponentBase,
}

impl Svg {
    pub fn from_bytes(
        name: String,
        transform: Transform,
        data: &[u8],
        width: Option<f64>,
        height: Option<f64>,
    ) -> Result<Self> {
        let options = resvg::usvg::Options::default();
        let tree = resvg::usvg::Tree::from_data(data, &options)
            .map_err(|error| Error::Svg(error.to_string()))?;
        let source_size = tree.size();
        let (target_width, target_height) = target_dimensions(
            f64::from(source_size.width()),
            f64::from(source_size.height()),
            width,
            height,
        )?;
        let pixel_width = target_width.ceil() as u32;
        let pixel_height = target_height.ceil() as u32;
        let mut pixmap = resvg::tiny_skia::Pixmap::new(pixel_width, pixel_height)
            .ok_or_else(|| Error::InvalidInput("SVG target is too large".into()))?;
        let scale = resvg::tiny_skia::Transform::from_scale(
            pixel_width as f32 / source_size.width(),
            pixel_height as f32 / source_size.height(),
        );
        resvg::render(&tree, scale, &mut pixmap.as_mut());
        let surface = surface_from_rgba(pixel_width, pixel_height, pixmap.data(), true)?;
        let base = paint_image_surface(
            name,
            transform,
            &surface,
            f64::from(pixel_width),
            f64::from(pixel_height),
            Some(target_width),
            Some(target_height),
        )?;
        Ok(Self { base })
    }

    pub fn from_file(
        name: String,
        transform: Transform,
        path: impl AsRef<Path>,
        width: Option<f64>,
        height: Option<f64>,
    ) -> Result<Self> {
        Self::from_bytes(name, transform, &std::fs::read(path)?, width, height)
    }
}

impl Component for Svg {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

/// Paints a child through the alpha channel of another component.
pub struct Mask {
    base: ComponentBase,
}

impl Mask {
    pub fn new<M: Component, C: Component>(
        name: String,
        transform: Transform,
        width: f64,
        height: f64,
        mask: M,
        child: C,
    ) -> Self {
        let base = ComponentBase::new(
            Some(Property {
                name,
                width,
                height,
            }),
            Some(transform),
        );
        base.cr.set_source_surface(child.surface(), 0., 0.).unwrap();
        base.cr.mask_surface(mask.surface(), 0., 0.).unwrap();
        Self { base }
    }
}

impl Component for Mask {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

pub struct Text {
    base: ComponentBase,
}

impl Text {
    #[allow(clippy::too_many_arguments)]
    pub fn new(
        name: String,
        transform: Transform,
        content: String,                  // 内容
        color: Option<Color>,             // 颜色
        font_family: Option<String>,      // 字体
        font_weight: Option<i32>,         // 粗细
        font_size: Option<f64>,           // 字号
        h_align: Option<HorizontalAlign>, // 水平对齐方式
        v_align: Option<VerticalAlign>,   // 垂直对齐方式
        max_width: Option<f64>,           // 最大宽度
        line_spacing: Option<f64>,        // 行间距
        _word_spacing: Option<f64>,       // 字间距
    ) -> Self {
        let color = color.unwrap_or(Color::BLACK);
        let font_family = font_family.unwrap_or_default();
        let font_weight = font_weight.unwrap_or(500);
        let font_size = font_size.unwrap_or(10.);
        let h_align = h_align.unwrap_or(HorizontalAlign::Center);
        let v_align = v_align.unwrap_or(VerticalAlign::BaseLine);
        let max_width = max_width.unwrap_or(f64::NAN);
        let line_spacing = line_spacing.unwrap_or(0.);
        let _word_spacing = _word_spacing.unwrap_or(0.);

        let base = ComponentBase::new(
            Some(Property {
                name,
                width: f64::NAN,
                height: f64::NAN,
            }),
            Some(transform),
        );
        base.cr
            .set_source_rgba(color.r(), color.g(), color.b(), color.a());

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
        desc.set_size((font_size * pango::SCALE as f64 * 72. / 96.).round() as i32);
        layout.set_font_description(Some(&desc));

        if !max_width.is_nan() {
            layout.set_width((max_width * pango::SCALE as f64).round() as i32); // 设置界定框
            layout.set_ellipsize(EllipsizeMode::End); // 超出用省略号
        }

        layout.set_spacing((line_spacing * pango::SCALE as f64).round() as i32); // 行距

        // 水平对齐
        match h_align {
            HorizontalAlign::Left => layout.set_alignment(Alignment::Left),
            HorizontalAlign::Center => layout.set_alignment(Alignment::Center),
            HorizontalAlign::Right => layout.set_alignment(Alignment::Right),
        }

        pangocairo::update_layout(&base.cr, &layout);
        let (layout_width, layout_height) = layout.size(); // 获取实际大小
        let real_w = layout_width as f64 / pango::SCALE as f64;
        let real_h = layout_height as f64 / pango::SCALE as f64;

        // 水平对齐
        let x_move = match h_align {
            HorizontalAlign::Left => 0.,
            HorizontalAlign::Center => -real_w / 2.,
            HorizontalAlign::Right => -real_w,
        };
        // 垂直对齐
        let y_move = match v_align {
            VerticalAlign::BaseLine => -layout.baseline() as f64 / pango::SCALE as f64,
            VerticalAlign::Top => 0.,
            VerticalAlign::Center => -real_h / 2.,
            VerticalAlign::Bottom => -real_h,
        };
        // 移动
        base.cr.move_to(x_move, y_move);

        pangocairo::show_layout(&base.cr, &layout);

        // base.property.width = real_w;
        // base.property.height = real_h;

        Self { base }
    }
}

impl Component for Text {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

pub struct TextArea {
    base: ComponentBase,
}

impl TextArea {
    #[allow(clippy::too_many_arguments)]
    pub fn new(
        name: String,
        transform: Transform,
        width: f64,
        height: f64,
        content: String,                  // 内容
        color: Option<Color>,             // 颜色
        font_family: Option<String>,      // 字体
        font_weight: Option<i32>,         // 粗细
        font_size: Option<f64>,           // 字号
        h_align: Option<HorizontalAlign>, // 水平对齐方式
        v_align: Option<VerticalAlign>,   // 垂直对齐方式
        line_spacing: Option<f64>,        // 行间距
        _word_spacing: Option<f64>,       // 字间距
    ) -> Self {
        let color = color.unwrap_or(Color::BLACK);
        let font_family = font_family.unwrap_or_default();
        let font_weight = font_weight.unwrap_or(500);
        let font_size = font_size.unwrap_or(10.);
        let h_align = h_align.unwrap_or(HorizontalAlign::Center);
        let v_align = v_align.unwrap_or(VerticalAlign::BaseLine);
        let line_spacing = line_spacing.unwrap_or(0.);
        let _word_spacing = _word_spacing.unwrap_or(0.);

        let base = ComponentBase::new(
            Some(Property {
                name,
                width,
                height,
            }),
            Some(transform),
        );
        base.cr
            .set_source_rgba(color.r(), color.g(), color.b(), color.a());

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
        desc.set_size((font_size * pango::SCALE as f64 * 72. / 96.).round() as i32);
        layout.set_font_description(Some(&desc));

        layout.set_width((width * pango::SCALE as f64).round() as i32); // 设置界定框
        layout.set_height((height * pango::SCALE as f64).round() as i32); // 设置界定框

        layout.set_wrap(WrapMode::WordChar); // 换行方式
        layout.set_ellipsize(EllipsizeMode::End); // 超出用省略号
        layout.set_spacing((line_spacing * pango::SCALE as f64).round() as i32); // 行距

        // 水平对齐
        match h_align {
            HorizontalAlign::Left => layout.set_alignment(Alignment::Left),
            HorizontalAlign::Center => layout.set_alignment(Alignment::Center),
            HorizontalAlign::Right => layout.set_alignment(Alignment::Right),
        }

        pangocairo::update_layout(&base.cr, &layout);
        let (layout_width, layout_height) = layout.size(); // 获取实际大小
        let _real_w = layout_width as f64 / pango::SCALE as f64;
        let real_h = layout_height as f64 / pango::SCALE as f64;

        // 垂直对齐
        let y_move = match v_align {
            VerticalAlign::BaseLine => 0.,
            VerticalAlign::Top => 0.,
            VerticalAlign::Center => (height - real_h) / 2.,
            VerticalAlign::Bottom => height - real_h,
        };
        // 移动
        base.cr.move_to(0., y_move);

        pangocairo::show_layout(&base.cr, &layout);

        Self { base }
    }
}

impl Component for TextArea {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

/// Tiles components inside a clipped rectangular region.
pub struct Repeat {
    base: ComponentBase,
}

impl Repeat {
    pub fn new(name: String, transform: Transform, width: f64, height: f64) -> Self {
        let base = ComponentBase::new(
            Some(Property {
                name,
                width,
                height,
            }),
            Some(transform),
        );
        Self { base }
    }

    pub fn add_child<T: Component>(&self, child: T) {
        let child_width = child.width();
        let child_height = child.height();
        if child_width <= 0.
            || child_height <= 0.
            || !child_width.is_finite()
            || !child_height.is_finite()
            || self.base.property.width <= 0.
            || self.base.property.height <= 0.
        {
            return;
        }

        self.base.cr.save().unwrap();
        self.base
            .cr
            .rectangle(0., 0., self.base.property.width, self.base.property.height);
        self.base.cr.clip();
        let mut y = 0.;
        while y < self.base.property.height {
            let mut x = 0.;
            while x < self.base.property.width {
                self.base
                    .cr
                    .set_source_surface(child.surface(), x, y)
                    .unwrap();
                self.base.cr.paint().unwrap();
                x += child_width;
            }
            y += child_height;
        }
        self.base.cr.restore().unwrap();
    }
}

impl Component for Repeat {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}

pub struct Group {
    base: ComponentBase,
}

impl Group {
    pub fn new(name: String, transform: Transform) -> Self {
        let base = ComponentBase::new(
            Some(Property {
                name,
                width: f64::NAN,
                height: f64::NAN,
            }),
            Some(transform),
        );
        Self { base }
    }
    pub fn add_child<T: Component>(&self, child: T) {
        self.base.cr.save().unwrap();
        self.base
            .cr
            .set_source_surface(child.surface(), 0., 0.)
            .unwrap();
        self.base.cr.paint().unwrap();
        self.base.cr.restore().unwrap();
    }
}

impl Component for Group {
    fn surface(&self) -> &RecordingSurface {
        self.base.surface()
    }
    fn property(&self) -> &Property {
        self.base.property()
    }
}
