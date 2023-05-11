pub mod component;
pub mod renderer;
pub mod types;

#[cfg(test)]
mod tests {
    use crate::types::{Transform, Color, Anchor};
    use crate::component::{Circle, Text};
    use crate::renderer::{RendererImage, Unit};

    #[test]
    fn it_works() {
        let _c = Circle::new(
            String::from("test"),
            Transform {
                x: 50.,
                y: 50.,
                anchor: Anchor::CC,
                ..Default::default()
            },
            100., 100.,
            Color::RED,
        );
        let str = Text::new(
            String::from("test"),
            Transform {
                x: 256.,
                y: 256.,
                anchor: Anchor::LT,
                ..Default::default()
            },
            String::from("喵喵喵"),
            Color::RED,
            None,
            None,
            Some(40.),
            None,
            None,
            None,
            None,
            None,
        );
        let r = RendererImage::new(512., 512., Unit::Pixel, 72.);
        r.render(str.surface());
        let mut f = std::fs::File::create("foo.png").unwrap(); // todo
        r.png_to_stream(&mut f);
    }
}
