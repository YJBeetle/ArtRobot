pub mod component;
pub mod renderer;
pub mod types;

#[cfg(test)]
mod tests {
    use super::*;
    use types::{Transform, Color, Anchor};
    use component::{Circle};
    use renderer::{RendererImage, Unit};

    #[test]
    fn it_works() {
        let s = Circle::new(
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
        let r = RendererImage::new(512., 512., Unit::Pixel, 72.);
        r.render(s.surface());
        let mut f = std::fs::File::create("foo.png").unwrap(); // todo
        r.png_to_stream(&mut f);
    }
}
