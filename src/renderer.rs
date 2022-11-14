use cairo::{Surface, SvgSurface, PdfSurface, ImageSurface, Context};

fn millimeter2inch(millimeter: f64) -> f64 { millimeter / 25.4 }

fn pixel2inch(pixel: f64, ppi: f64) -> f64 { pixel / ppi }

pub enum Unit {
    Pixel,
    Inch,
    Millimeter,
    Centimeter,
}

pub enum ImageType {
    Pixmap,
    Png,
    Webp,
    Jpeg,
}

pub enum OutputType {
    Svg,
    Pdf,
    Image(ImageType),
}

enum SurfaceType {
    Svg(SvgSurface),
    Pdf(PdfSurface),
    Image(ImageSurface, ImageType),
}

impl SurfaceType {
    fn get_surface(&self) -> &Surface {
        match &self {
            SurfaceType::Svg(s) => s,
            SurfaceType::Pdf(s) => s,
            SurfaceType::Image(s, _) => s,
        }
    }
}

struct WriteStreamToData {
    data: Vec<u8>,
}

impl std::io::Write for WriteStreamToData {
    fn write(&mut self, buf: &[u8]) -> std::io::Result<usize> {
        let mut b = Vec::from(buf);
        let ret = Ok(buf.len());
        self.data.append(&mut b);
        ret
    }
    fn flush(&mut self) -> std::io::Result<()> {
        self.data.flush()
    }
}

impl WriteStreamToData {
    pub fn new() -> Self {
        Self { data: Vec::new() }
    }
}

pub struct Renderer {
    data: WriteStreamToData,
    surface: SurfaceType,
    cr: Context,
}

impl Renderer {
    pub fn new(
        output_format: OutputType,
        width_with_unit: f64,
        height_with_unit: f64,
        unit: Unit,
        ppi: f64,
    ) -> Self {
        let width: f64;
        let height: f64;
        let scale: f64;
        match unit {
            Unit::Pixel => {
                width = pixel2inch(width_with_unit, ppi);
                height = pixel2inch(height_with_unit, ppi);
                scale = pixel2inch(1., ppi);
            }
            Unit::Inch => {
                width = width_with_unit;
                height = height_with_unit;
                scale = 1.;
            }
            Unit::Millimeter => {
                width = millimeter2inch(width_with_unit);
                height = millimeter2inch(height_with_unit);
                scale = millimeter2inch(1.);
            }
            Unit::Centimeter => {
                width = millimeter2inch(width_with_unit) * 10.;
                height = millimeter2inch(height_with_unit) * 10.;
                scale = millimeter2inch(1.) * 10.;
            }
        }

        let data = WriteStreamToData::new();
        let surface;
        let w = std::fs::File::create("foo").unwrap(); // todo
        match output_format {
            OutputType::Svg => {
                surface = SurfaceType::Svg(SvgSurface::for_stream(width * ppi, height * ppi, w).unwrap());
            }
            OutputType::Pdf => {
                let s = PdfSurface::for_stream(width * ppi, height * ppi, w).unwrap();
                s.set_fallback_resolution(ppi, ppi); //设置分辨率
                surface = SurfaceType::Pdf(s);
            }
            OutputType::Image(image_format) => {
                surface = SurfaceType::Image(
                    ImageSurface::create(cairo::Format::ARgb32, (width * ppi).round() as i32, (height * ppi).round() as i32).unwrap(),
                    image_format,
                );
            }
        }

        let cr = cairo::Context::new(surface.get_surface()).unwrap();
        cr.scale(scale * ppi, scale * ppi);
        // cr.show_page().unwrap(); // 多页

        Self {
            data,
            surface,
            cr,
        }
    }

    pub fn render(&mut self, surface: &Surface) {
        self.cr.set_source_surface(surface, 0., 0.).unwrap();
        self.cr.paint().unwrap();

        match &self.surface {
            SurfaceType::Svg(surface) => { surface.finish(); }
            SurfaceType::Pdf(surface) => { surface.finish(); }
            SurfaceType::Image(surface, img_format) => {
                match img_format {
                    ImageType::Pixmap => {
                        let mut d = Vec::new(); // todo
                        surface.with_data(|v: &[u8]| { d = Vec::from(v) }).unwrap();
                    }
                    ImageType::Png => {
                        // let mut f = std::fs::File::create("foo.png").unwrap(); // todo
                        surface.write_to_png(&mut self.data).unwrap();
                    }
                    ImageType::Webp => {} // todo
                    ImageType::Jpeg => {} // todo
                }
            }
        }
    }
}
