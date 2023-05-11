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

pub struct Rectangle {}


pub struct RectangleRound {}


pub struct Circle {
    base: Base,
}

impl Circle {
    pub fn new(name: String, transform: Transform, width: f64, height: f64, color: Color) -> Self {
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
