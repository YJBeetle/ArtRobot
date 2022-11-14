use cairo::{Surface, SvgSurface, PdfSurface, ImageSurface, Context};

fn millimeter2inch(millimeter: f64) -> f64 { millimeter / 25.4 }

fn pixel2inch(pixel: f64, ppi: f64) -> f64 { pixel / ppi }

pub enum Unit {
    Pixel,
    Inch,
    Millimeter,
    Centimeter,
}

struct Base {
    width: f64,
    height: f64,
    ppi: f64,
    scale: f64,
}

impl Base {
    fn new(
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
        Self { width, height, ppi, scale }
    }
    fn render(&self, surface: &Surface, input_surface: &Surface) {
        let cr = Context::new(surface).unwrap();
        cr.scale(self.scale * self.ppi, self.scale * self.ppi);

        cr.set_source_surface(input_surface, 0., 0.).unwrap();
        cr.paint().unwrap();
    }
}

pub struct RendererSvg {
    base: Base,
    surface: SvgSurface,
}

impl RendererSvg {
    pub fn new<W: std::io::Write + 'static>(
        width_with_unit: f64,
        height_with_unit: f64,
        unit: Unit,
        ppi: f64,
        stream: W,
    ) -> Self {
        let base = Base::new(width_with_unit, height_with_unit, unit, ppi);
        let surface = SvgSurface::for_stream(base.width * base.ppi, base.height * base.ppi, stream).unwrap();
        Self { base, surface }
    }
    pub fn render(&self, input_surface: &Surface) {
        self.base.render(&self.surface, input_surface);
        self.surface.finish();
    }
}


pub struct RendererPdf {
    base: Base,
    surface: PdfSurface,
}

impl RendererPdf {
    pub fn new<W: std::io::Write + 'static>(
        width_with_unit: f64,
        height_with_unit: f64,
        unit: Unit,
        ppi: f64,
        stream: W,
    ) -> Self {
        let base = Base::new(width_with_unit, height_with_unit, unit, ppi);
        let surface = PdfSurface::for_stream(base.width * base.ppi, base.height * base.ppi, stream).unwrap();
        surface.set_fallback_resolution(base.ppi, base.ppi); //设置分辨率
        Self { base, surface }
    }
    pub fn render(&self, input_surface: &Surface) {
        self.base.render(&self.surface, input_surface);
        // cr.show_page().unwrap(); // PDF多页
        self.surface.finish();
    }
}


pub enum ImageType {
    Pixmap,
    Png,
    Webp,
    Jpeg,
}

pub struct RendererImage {
    base: Base,
    surface: ImageSurface,
}


impl RendererImage {
    pub fn new(
        width_with_unit: f64,
        height_with_unit: f64,
        unit: Unit,
        ppi: f64,
    ) -> Self {
        let base = Base::new(width_with_unit, height_with_unit, unit, ppi);
        let surface = ImageSurface::create(cairo::Format::ARgb32, (base.width * base.ppi).round() as i32, (base.height * base.ppi).round() as i32).unwrap();
        Self { base, surface }
    }
    pub fn render(&self, input_surface: &Surface) {
        self.base.render(&self.surface, input_surface);
    }
    pub fn pixmap(&self) -> Vec<u8> {
        let mut d = Vec::new();
        self.surface.with_data(|v: &[u8]| { d = Vec::from(v) }).unwrap();
        d
    }
    pub fn pixmap_to_stream<W: std::io::Write>(&self, stream: &mut W) {
        self.surface.with_data(
            |v: &[u8]| {
                stream.write(v).unwrap();
            }
        ).unwrap();
    }
    pub fn png_to_stream<W: std::io::Write>(&self, stream: &mut W) {
        self.surface.write_to_png(stream).unwrap();
    }
    pub fn webp(&self) {
        // todo
    }
    pub fn jpeg(&self) {
        // todo
    }
}
