pub mod component;
pub mod renderer;
pub mod types;

#[cfg(test)]
mod tests {
    use crate::types::{Transform, Color, Anchor};
    use crate::component::{Circle, Component, Group, Text, TextArea};
    use crate::renderer::{RendererImage, Unit};

    #[test]
    fn it_works() {
        let c = Circle::new(
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
                ..Default::default()
            },
            String::from("喵喵喵"),
            Some(Color::BLUE),
            None,
            None,
            Some(40.),
            None,
            None,
            None,
            None,
            None,
        );
        let str2 = TextArea::new(
            String::from("test"),
            Transform {
                x: 256.,
                y: 256.,
                anchor: Anchor::CC,
                ..Default::default()
            },
            100.,
            100.,
            String::from("喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵"),
            None,
            None,
            None,
            Some(40.),
            None,
            None,
            None,
            None,
        );
        let g: Group = Group::new(
            String::from("group"),
            Transform {
                x: 0.,
                y: 0.,
                ..Default::default()
            },
        );
        g.add_child(c);
        g.add_child(str);
        g.add_child(str2);
        let r = RendererImage::new(512., 512., Unit::Pixel, 72.);
        r.render(g.surface());
        let mut f = std::fs::File::create("foo.png").unwrap(); // todo
        r.png_to_stream(&mut f);
    }
}
