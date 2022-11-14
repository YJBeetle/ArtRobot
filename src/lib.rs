pub mod component;
pub mod renderer;
pub mod types;

// pub fn add(left: usize, right: usize) -> usize {
//     left + right
//     // let xx = cairo::RecordingSurface::create(cairo::Content::Alpha, None);
// }

#[cfg(test)]
mod tests {
    use types::{Transform, Color, Anchor};
    use component::{Base, Circle};
    use renderer::{Renderer, OutputType, ImageType, Unit};

    use super::*;

    #[test]
    fn it_works() {
        // let result = add(2, 2);
        // assert_eq!(result, 4);
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
        let mut r = Renderer::new(OutputType::Image(ImageType::Png), 512., 512., Unit::Pixel, 72.);
        r.render(s.surface());
    }
}
