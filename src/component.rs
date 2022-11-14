use std::f64::consts::PI;
use cairo::{RecordingSurface, Context};
use crate::types::{Color, Property, Transform};

pub trait Base {
    fn property(&self) -> &Property;
    fn surface(&self) -> &RecordingSurface;
}


pub struct Rectangle {}


pub struct RectangleRound {}


pub struct Circle {
    surface: RecordingSurface,
    cr: Context,
    property: Property,
    transform: Transform,
}

impl Base for Circle {
    fn property(&self) -> &Property { &self.property }
    fn surface(&self) -> &RecordingSurface { &self.surface }
}

impl Circle {
    pub fn new(name: String, transform: Transform, width: f64, height: f64, color: Color) -> Self {
        let surface = RecordingSurface::create(cairo::Content::Alpha, None).unwrap();
        let cr = Context::new(&surface).unwrap();
        let property = Property { name, width, height };
        let ret = Self {
            surface,
            cr,
            property,
            transform,
        };
        ret.transform.apply(&ret.property, &ret.cr);
        ret.cr.set_source_rgba(color.r(), color.g(), color.b(), color.a());
        ret.cr.scale(width / 2., height / 2.);
        ret.cr.arc(1., 1., 1., 0., 2. * PI);
        ret.cr.fill().unwrap();
        ret
    }
}


pub struct Svg {}


pub struct Image {}


pub struct Mask {}


pub struct Text {}


pub struct TextArea {}


pub struct Repeat {}


pub struct Group {}
