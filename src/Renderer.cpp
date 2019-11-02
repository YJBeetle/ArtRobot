#ifdef WIN32
#include <fcntl.h>
#endif

#include "default.h"
#include "Renderer.h"

namespace Render
{

Renderer::Renderer(double __width,
                   double __height,
                   unitType __unit,
                   double __ppi)
{
    double scale;
    switch (__unit)
    {
    case PX:
        this->surfaceWidth = PT2IN(__width);
        this->surfaceHeight = PT2IN(__height);
        scale = PT2IN(1);
        break;
    case IN:
        this->surfaceWidth = __width;
        this->surfaceHeight = __height;
        scale = 1;
        break;
    case MM:
        this->surfaceWidth = MM2IN(__width);
        this->surfaceHeight = MM2IN(__height);
        scale = MM2IN(1);
        break;
    case CM:
        this->surfaceWidth = MM2IN(__width) * 10;
        this->surfaceHeight = MM2IN(__height) * 10;
        scale = MM2IN(1) * 10;
        break;
    }
    ppi = __ppi;

    surface = cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, NULL);
    cr = cairo_create(surface);                //创建画笔
    cairo_scale(cr, scale * ppi, scale * ppi); //缩放画笔
}

Renderer::~Renderer()
{
    cairo_destroy(cr);              //回收画笔
    cairo_surface_destroy(surface); //回收介质
}

void Renderer::render(cairo_surface_t *__surface)
{
    cairo_set_source_surface(cr, __surface, 0.0, 0.0);
    cairo_paint(cr);
}

cairo_status_t writeCairo(void *closure, const unsigned char *data, unsigned int length)
{
    fwrite(data, length, 1, (FILE *)closure);
    return CAIRO_STATUS_SUCCESS;
}

void Renderer::saveToFile(string outputPath, OutputType outputType)
{
    FILE *outputFile;
    if (!outputPath.empty())
    {
        outputFile = fopen(outputPath.c_str(), "wb");
    }
    else
    {
#ifdef WIN32
        _setmode(_fileno(stdout), O_BINARY);
#endif
        outputFile = stdout;
    }

    cairo_surface_t *outputSurface;
    switch (outputType)
    {
    case OutputTypePdf:
        outputSurface = cairo_pdf_surface_create_for_stream(writeCairo,
                                                            (void *)outputFile,
                                                            surfaceWidth * ppi,
                                                            (surfaceHeight)*ppi); //默认单位是mm，所以需要mm转inch
        // cairo_surface_set_fallback_resolution(surface, 300, 300);                 //设置分辨率
        // cairo_show_page(cr);                                                      // 多页
        break;
    case OutputTypePng:
        outputSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
                                                   surfaceWidth * ppi,
                                                   surfaceHeight * ppi); //默认单位pt
        break;
    case OutputTypeSvg:
    default:
        outputSurface = cairo_svg_surface_create_for_stream(writeCairo,
                                                            (void *)outputFile,
                                                            surfaceWidth * ppi,
                                                            surfaceHeight * ppi); //默认单位pt
        break;
    }

    cairo_t *outputCr = cairo_create(outputSurface);
    cairo_set_source_surface(outputCr, surface, 0.0, 0.0);
    cairo_paint(outputCr);
    cairo_destroy(outputCr); //回收画笔

    if (outputType == OutputTypePng)
        cairo_surface_write_to_png_stream(outputSurface, writeCairo, (void *)outputFile);

    cairo_surface_destroy(outputSurface); //回收介质

    if (outputFile != stdout)
        fclose(outputFile);
}

} // namespace Render
