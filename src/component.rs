use std::f64::consts::PI;
use cairo::{RecordingSurface, Context};
use crate::types::{Color, Property, Transform};

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


pub struct Text {}


pub struct TextArea {}


pub struct Repeat {}


pub struct Group {}
