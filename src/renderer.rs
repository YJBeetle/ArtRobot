use std::io::Write;
use std::path::Path;

use cairo::{Context, ImageSurface, PdfSurface, Surface, SvgSurface};
use image::ImageEncoder;

use crate::error::{Error, Result};

fn millimeter_to_inch(millimeter: f64) -> f64 {
    millimeter / 25.4
}

fn pixel_to_inch(pixel: f64, ppi: f64) -> f64 {
    pixel / ppi
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum Unit {
    Pixel,
    Inch,
    Millimeter,
    Centimeter,
}

struct RendererBase {
    width: f64,
    height: f64,
    ppi: f64,
    scale: f64,
}

impl RendererBase {
    fn new(width_with_unit: f64, height_with_unit: f64, unit: Unit, ppi: f64) -> Self {
        assert!(
            width_with_unit > 0. && height_with_unit > 0.,
            "renderer dimensions must be positive"
        );
        assert!(
            ppi > 0. && ppi.is_finite(),
            "renderer PPI must be positive and finite"
        );
        let (width, height, scale) = match unit {
            Unit::Pixel => (
                pixel_to_inch(width_with_unit, ppi),
                pixel_to_inch(height_with_unit, ppi),
                pixel_to_inch(1., ppi),
            ),
            Unit::Inch => (width_with_unit, height_with_unit, 1.),
            Unit::Millimeter => (
                millimeter_to_inch(width_with_unit),
                millimeter_to_inch(height_with_unit),
                millimeter_to_inch(1.),
            ),
            Unit::Centimeter => (
                millimeter_to_inch(width_with_unit) * 10.,
                millimeter_to_inch(height_with_unit) * 10.,
                millimeter_to_inch(1.) * 10.,
            ),
        };
        Self {
            width,
            height,
            ppi,
            scale,
        }
    }

    fn pixel_width(&self) -> i32 {
        (self.width * self.ppi).round() as i32
    }

    fn pixel_height(&self) -> i32 {
        (self.height * self.ppi).round() as i32
    }

    fn render(&self, surface: &Surface, input_surface: &Surface) -> Result<()> {
        let cr = Context::new(surface)?;
        cr.scale(self.scale * self.ppi, self.scale * self.ppi);
        cr.set_source_surface(input_surface, 0., 0.)?;
        cr.paint()?;
        Ok(())
    }
}

pub struct RendererSvg {
    base: RendererBase,
    surface: SvgSurface,
}

impl RendererSvg {
    pub fn new<W: Write + 'static>(
        width_with_unit: f64,
        height_with_unit: f64,
        unit: Unit,
        ppi: f64,
        stream: W,
    ) -> Self {
        let base = RendererBase::new(width_with_unit, height_with_unit, unit, ppi);
        let surface = SvgSurface::for_stream(base.width * base.ppi, base.height * base.ppi, stream)
            .expect("failed to create SVG surface");
        Self { base, surface }
    }

    pub fn render(&self, input_surface: &Surface) -> Result<()> {
        self.base.render(&self.surface, input_surface)?;
        self.surface.finish();
        Ok(())
    }
}

pub struct RendererPdf {
    base: RendererBase,
    surface: PdfSurface,
}

impl RendererPdf {
    pub fn new<W: Write + 'static>(
        width_with_unit: f64,
        height_with_unit: f64,
        unit: Unit,
        ppi: f64,
        stream: W,
    ) -> Self {
        let base = RendererBase::new(width_with_unit, height_with_unit, unit, ppi);
        let surface = PdfSurface::for_stream(base.width * base.ppi, base.height * base.ppi, stream)
            .expect("failed to create PDF surface");
        surface.set_fallback_resolution(base.ppi, base.ppi);
        Self { base, surface }
    }

    pub fn render(&self, input_surface: &Surface) -> Result<()> {
        self.base.render(&self.surface, input_surface)?;
        self.surface.finish();
        Ok(())
    }
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum ImageType {
    Pixmap,
    Png,
    Webp,
    Jpeg,
}

pub struct RendererImage {
    base: RendererBase,
    surface: ImageSurface,
}

impl RendererImage {
    pub fn new(width_with_unit: f64, height_with_unit: f64, unit: Unit, ppi: f64) -> Self {
        let base = RendererBase::new(width_with_unit, height_with_unit, unit, ppi);
        let surface = ImageSurface::create(
            cairo::Format::ARgb32,
            base.pixel_width(),
            base.pixel_height(),
        )
        .expect("failed to create image surface");
        Self { base, surface }
    }

    pub fn width(&self) -> u32 {
        self.base.pixel_width() as u32
    }

    pub fn height(&self) -> u32 {
        self.base.pixel_height() as u32
    }

    pub fn render(&self, input_surface: &Surface) -> Result<()> {
        self.base.render(&self.surface, input_surface)
    }

    /// Returns Cairo's native-endian premultiplied ARGB32 pixel buffer.
    pub fn pixmap(&self) -> Vec<u8> {
        let mut data = Vec::new();
        self.surface
            .with_data(|bytes| data.extend_from_slice(bytes))
            .expect("failed to access image surface data");
        data
    }

    pub fn pixmap_to_stream<W: Write>(&self, stream: &mut W) -> Result<()> {
        stream.write_all(&self.pixmap())?;
        Ok(())
    }

    pub fn png(&self) -> Result<Vec<u8>> {
        let mut data = Vec::new();
        self.surface
            .write_to_png(&mut data)
            .map_err(|error| Error::InvalidInput(format!("failed to encode PNG: {error}")))?;
        Ok(data)
    }

    pub fn png_to_stream<W: Write>(&self, stream: &mut W) -> Result<()> {
        stream.write_all(&self.png()?)?;
        Ok(())
    }

    pub fn jpeg(&self, quality: u8) -> Result<Vec<u8>> {
        let rgba = self.rgba_pixels();
        let rgb: Vec<u8> = rgba
            .chunks_exact(4)
            .flat_map(|pixel| pixel[..3].iter().copied())
            .collect();
        let mut data = Vec::new();
        image::codecs::jpeg::JpegEncoder::new_with_quality(&mut data, quality).encode(
            &rgb,
            self.width(),
            self.height(),
            image::ExtendedColorType::Rgb8,
        )?;
        Ok(data)
    }

    pub fn jpeg_to_stream<W: Write>(&self, stream: &mut W, quality: u8) -> Result<()> {
        stream.write_all(&self.jpeg(quality)?)?;
        Ok(())
    }

    pub fn webp(&self) -> Result<Vec<u8>> {
        let rgba = self.rgba_pixels();
        let mut data = Vec::new();
        image::codecs::webp::WebPEncoder::new_lossless(&mut data).write_image(
            &rgba,
            self.width(),
            self.height(),
            image::ExtendedColorType::Rgba8,
        )?;
        Ok(data)
    }

    pub fn webp_to_stream<W: Write>(&self, stream: &mut W) -> Result<()> {
        stream.write_all(&self.webp()?)?;
        Ok(())
    }

    pub fn encoded(&self, image_type: ImageType) -> Result<Vec<u8>> {
        match image_type {
            ImageType::Pixmap => Ok(self.pixmap()),
            ImageType::Png => self.png(),
            ImageType::Webp => self.webp(),
            ImageType::Jpeg => self.jpeg(100),
        }
    }

    pub fn save(&self, path: impl AsRef<Path>, image_type: ImageType) -> Result<()> {
        std::fs::write(path, self.encoded(image_type)?)?;
        Ok(())
    }

    fn rgba_pixels(&self) -> Vec<u8> {
        let pixmap = self.pixmap();
        let mut rgba = Vec::with_capacity(pixmap.len());
        for pixel in pixmap.chunks_exact(4) {
            let argb = u32::from_ne_bytes([pixel[0], pixel[1], pixel[2], pixel[3]]);
            let alpha = (argb >> 24) as u8;
            let unpremultiply = |channel: u8| {
                if alpha == 0 {
                    0
                } else {
                    ((u16::from(channel) * 255 / u16::from(alpha)).min(255)) as u8
                }
            };
            rgba.extend_from_slice(&[
                unpremultiply((argb >> 16) as u8),
                unpremultiply((argb >> 8) as u8),
                unpremultiply(argb as u8),
                alpha,
            ]);
        }
        rgba
    }
}
