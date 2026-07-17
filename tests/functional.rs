use std::io::Write;
use std::sync::{Arc, Mutex};

use art_robot::component::{Circle, Component, Image, Mask, Rectangle, Repeat, Svg};
use art_robot::renderer::{ImageType, RendererImage, RendererPdf, RendererSvg, Unit};
use art_robot::types::{Anchor, Color, Transform};

fn left_top() -> Transform {
    Transform {
        anchor: Anchor::LT,
        ..Transform::default()
    }
}

fn pixel(pixmap: &[u8], width: usize, x: usize, y: usize) -> (u8, u8, u8, u8) {
    let start = (y * width + x) * 4;
    let argb = u32::from_ne_bytes(pixmap[start..start + 4].try_into().unwrap());
    (
        (argb >> 16) as u8,
        (argb >> 8) as u8,
        argb as u8,
        (argb >> 24) as u8,
    )
}

fn render(component: &impl Component, width: f64, height: f64) -> RendererImage {
    let renderer = RendererImage::new(width, height, Unit::Pixel, 72.);
    renderer.render(component.surface()).unwrap();
    renderer
}

#[test]
fn raster_formats_round_trip() {
    let source = Rectangle::new("source".into(), left_top(), 32., 24., Color::AQUA);
    let renderer = render(&source, 32., 24.);

    let png = renderer.encoded(ImageType::Png).unwrap();
    assert!(png.starts_with(&[0x89, b'P', b'N', b'G', 0x0d, 0x0a, 0x1a, 0x0a]));
    let jpeg = renderer.encoded(ImageType::Jpeg).unwrap();
    assert!(jpeg.starts_with(&[0xff, 0xd8, 0xff]));
    let webp = renderer.encoded(ImageType::Webp).unwrap();
    assert_eq!(&webp[..4], b"RIFF");
    assert_eq!(&webp[8..12], b"WEBP");

    for encoded in [&png, &jpeg, &webp] {
        let decoded = Image::from_bytes("decoded".into(), left_top(), encoded, None, None).unwrap();
        let output = render(&decoded, 32., 24.).pixmap();
        let (red, green, blue, alpha) = pixel(&output, 32, 16, 12);
        assert!(red <= 4 && green >= 251 && blue >= 251 && alpha == 255);
    }
}

#[test]
fn raw_image_preserves_size_and_color() {
    let bgra = [255, 0, 0, 255, 255, 0, 0, 255];
    let image = Image::from_bgra(
        "raw".into(),
        left_top(),
        &bgra,
        2,
        1,
        8,
        true,
        Some(4.),
        None,
    )
    .unwrap();
    assert_eq!(image.width(), 4.);
    assert_eq!(image.height(), 2.);
    assert_eq!(
        pixel(&render(&image, 4., 2.).pixmap(), 4, 2, 1),
        (0, 0, 255, 255)
    );
}

#[test]
fn svg_mask_and_repeat_render() {
    let svg = Svg::from_bytes(
        "svg".into(),
        left_top(),
        br##"<svg xmlns="http://www.w3.org/2000/svg" width="12" height="8"><rect width="12" height="8" fill="#00ffff"/></svg>"##,
        Some(24.),
        Some(16.),
    )
    .unwrap();
    assert_eq!(
        pixel(&render(&svg, 24., 16.).pixmap(), 24, 12, 8),
        (0, 255, 255, 255)
    );

    let mask_shape = Rectangle::new("mask".into(), left_top(), 8., 16., Color::WHITE);
    let child = Rectangle::new("child".into(), left_top(), 16., 16., Color::FUCHSIA);
    let mask = Mask::new("masked".into(), left_top(), 16., 16., &mask_shape, &child);
    let masked = render(&mask, 16., 16.).pixmap();
    assert_eq!(pixel(&masked, 16, 4, 8), (255, 0, 255, 255));
    assert_eq!(pixel(&masked, 16, 12, 8).3, 0);

    let tile = Rectangle::new("tile".into(), left_top(), 2., 3., Color::RED);
    let repeat = Repeat::new("repeat".into(), left_top(), 5., 7.);
    repeat.add_child(&tile);
    let repeated = render(&repeat, 8., 8.).pixmap();
    assert_eq!(pixel(&repeated, 8, 4, 6), (255, 0, 0, 255));
    assert_eq!(pixel(&repeated, 8, 6, 6).3, 0);
}

#[test]
fn transforms_and_color_parsing_match_cpp_behavior() {
    assert_eq!(Color::new("#0f08").rgba(), (0., 1., 0., 136. / 255.));
    assert_eq!(
        Color::new("#123456").rgba(),
        (18. / 255., 52. / 255., 86. / 255., 1.)
    );

    let circle = Circle::new(
        "circle".into(),
        Transform {
            x: 10.,
            y: 10.,
            anchor: Anchor::CC,
            ..Transform::default()
        },
        8.,
        8.,
        Color::RED,
    );
    let pixels = render(&circle, 20., 20.).pixmap();
    assert_eq!(pixel(&pixels, 20, 10, 10), (255, 0, 0, 255));
    assert_eq!(pixel(&pixels, 20, 1, 1).3, 0);
}

#[derive(Clone, Default)]
struct SharedWriter(Arc<Mutex<Vec<u8>>>);

impl Write for SharedWriter {
    fn write(&mut self, data: &[u8]) -> std::io::Result<usize> {
        self.0.lock().unwrap().extend_from_slice(data);
        Ok(data.len())
    }

    fn flush(&mut self) -> std::io::Result<()> {
        Ok(())
    }
}

#[test]
fn vector_renderers_and_units_work() {
    let source = Rectangle::new("source".into(), left_top(), 10., 10., Color::BLACK);
    let pdf_output = SharedWriter::default();
    let pdf = RendererPdf::new(25.4, 25.4, Unit::Millimeter, 72., pdf_output.clone());
    pdf.render(source.surface()).unwrap();
    assert!(pdf_output.0.lock().unwrap().starts_with(b"%PDF-"));

    let svg_output = SharedWriter::default();
    let svg = RendererSvg::new(1., 1., Unit::Inch, 72., svg_output.clone());
    svg.render(source.surface()).unwrap();
    assert!(String::from_utf8_lossy(&svg_output.0.lock().unwrap()).contains("<svg"));

    let pixels = RendererImage::new(2.54, 1.27, Unit::Centimeter, 100.);
    assert_eq!((pixels.width(), pixels.height()), (100, 50));
}

#[test]
fn invalid_assets_return_errors() {
    assert!(Image::from_bytes("bad".into(), left_top(), b"not an image", None, None).is_err());
    assert!(Svg::from_bytes("bad".into(), left_top(), b"not svg", None, None).is_err());
    assert!(
        Image::from_bgra("bad".into(), left_top(), &[0; 3], 1, 1, 4, true, None, None).is_err()
    );
}

#[test]
fn png_and_jpeg_fixtures_load_from_files() {
    let fixtures = ["img.png", "img.jpg", "img-Gray.jpg", "img-CMYK.jpg"];
    for fixture in fixtures {
        let path = std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("test")
            .join(fixture);
        let image = Image::from_file(fixture.into(), left_top(), path, None, None).unwrap();
        assert!(image.width() > 0. && image.height() > 0.);
        let pixels = render(&image, image.width(), image.height()).pixmap();
        assert!(pixels
            .chunks_exact(4)
            .any(|pixel| pixel.iter().any(|byte| *byte != 0)));
    }
}
