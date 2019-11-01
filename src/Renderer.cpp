#ifdef WIN32
#include <fcntl.h>
#endif

#include "default.h"
#include "Color.h"
#include "Renderer.h"

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

bool filecheck(const char *filename)
{
    FILE *file;
    file = fopen(filename, "rb");
    if (file)
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

cairo_status_t writeCairo(void *closure, const unsigned char *data, unsigned int length)
{
    fwrite(data, length, 1, (FILE *)closure);
    return CAIRO_STATUS_SUCCESS;
}

void Renderer::save(string outputPath, OutputType outputType)
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

int8_t Renderer::draw_rectangle(Color argb, double x, double y, double width, double height)
{
    cairo_save(cr); //保存画笔

    cairo_set_source_rgba(cr, argb.red(), argb.green(), argb.blue(), argb.alpha());
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);
    cairo_restore(cr); //还原画笔

    return 0;
}

int8_t Renderer::draw_text(const string &text,
                           const string &fontfile,
                           long face_index,
                           double font_size,
                           int8_t alignment,
                           Color argb,
                           double x, double y)
{
    if (!face_index)
        face_index = 0;

    cairo_save(cr); //保存画笔
    FT_Library ft_library;
    FT_Face ft_face;
    cairo_font_face_t *cr_face;
    if (FT_Init_FreeType(&ft_library))
    {
        fprintf(stderr, "Renderer::RendererTEXT: warning: FT_Init_FreeType failed.\n");
        return 4;
    }
    if (FT_New_Face(ft_library, fontfile.c_str(), face_index, &ft_face))
    {
        fprintf(stderr, "Renderer::RendererTEXT: error: FT_New_Face failed, maybe font not found.\n");
        return 5;
    }
    cr_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);
    cairo_set_font_face(cr, cr_face);
    //cairo_select_font_face (cr, family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, font_size);
    cairo_set_source_rgba(cr, argb.red(), argb.green(), argb.blue(), argb.alpha());

    cairo_text_extents_t extents;
    switch (alignment)
    {
    case 1:
        cairo_text_extents(cr, text.c_str(), &extents);
        cairo_move_to(cr, x - extents.width / 2, y);
        break;
    case 2:
        cairo_text_extents(cr, text.c_str(), &extents);
        cairo_move_to(cr, x - extents.width, y);
        break;
    case 0:
    default:
        cairo_move_to(cr, x, y);
        break;
    }

    //cairo_show_text (cr, text);
    cairo_text_path(cr, text.c_str());
    cairo_fill(cr);
    cairo_restore(cr); //还原画笔

    return 0;
}

int8_t Renderer::draw_svg(const string &svgfilename,
                          double x, double y,
                          double width, double height)
{
    if (!filecheck(svgfilename.c_str()))
    {
        fprintf(stderr, "Renderer::RendererSVG: warning: file not found: %s\n", svgfilename.c_str());
        return 2;
    }

    RsvgHandle *svg;
    svg = rsvg_handle_new_from_file(svgfilename.c_str(), NULL);

    cairo_save(cr); //保存画笔

    cairo_translate(cr, x, y);
    if (width || height)
    {
        unsigned int svg_width, svg_height;
        double scaleX, scaleY;
        RsvgDimensionData dimension_data;
        rsvg_handle_get_dimensions(svg, &dimension_data);
        svg_width = dimension_data.width;
        svg_height = dimension_data.height;
        scaleX = width / (double)svg_width;
        scaleY = height / (double)svg_height;
        cairo_scale(cr, scaleX, scaleY);
    }
    rsvg_handle_render_cairo(svg, cr);

    rsvg_handle_close(svg, NULL); //释放handle
    cairo_restore(cr);            //还原画笔

    return 0;
}

int8_t Renderer::draw_png(const string &pngfilename,
                          double x, double y,
                          double width, double height)
{
    if (!filecheck(pngfilename.c_str()))
    {
        fprintf(stderr, "Renderer::RendererPNG: warning: file not found: %s\n", pngfilename.c_str());
        return 2;
    }

    cairo_surface_t *img = NULL;
    img = cairo_image_surface_create_from_png(pngfilename.c_str());

    cairo_save(cr); //保存画笔

    cairo_translate(cr, x, y);
    if (width || height)
    {
        unsigned int png_width, png_height;
        double scaleX, scaleY;
        png_width = cairo_image_surface_get_width(img);
        png_height = cairo_image_surface_get_height(img);
        scaleX = width / (double)png_width;
        scaleY = height / (double)png_height;
        cairo_scale(cr, scaleX, scaleY);
    }
    cairo_set_source_surface(cr, img, 0, 0);
    cairo_paint(cr);

    cairo_surface_destroy(img); //回收PNG介质
    cairo_restore(cr);          //还原画笔

    return 0;
}
