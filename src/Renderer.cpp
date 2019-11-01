#ifdef WIN32
#include <fcntl.h>
#endif

#include "default.h"
#include "Color.h"
#include "Renderer.h"

Renderer::Renderer(const string &filename,
           OutputType type,
           double width,
           double height,
           unitType unit,
           double ppi)
    : out_file(NULL),
      surface_height(0),
      surface_width(0)
{
    if (!type)
        assert("Renderer::Init: error: Unknow type , Failure to initialize!\n");

    if (!filename.empty())
    {
        this->out_file = fopen(filename.c_str(), "wb");
    }
    else
    {
#ifdef WIN32
        _setmode(_fileno(stdout), O_BINARY);
#endif
        this->out_file = stdout;
    }

    this->surface_type = type;

    double scale;
    switch (unit)
    {
    case PX:
        this->surface_width = PT2IN(width);
        this->surface_height = PT2IN(height);
        scale = PT2IN(1);
        break;
    case IN:
        this->surface_width = width;
        this->surface_height = height;
        scale = 1;
        break;
    case MM:
        this->surface_width = MM2IN(width);
        this->surface_height = MM2IN(height);
        scale = MM2IN(1);
        break;
    case CM:
        this->surface_width = MM2IN(width) * 10;
        this->surface_height = MM2IN(height) * 10;
        scale = MM2IN(1) * 10;
        break;
    }

    if (!ppi)
        ppi = 72;

    switch (surface_type)
    {
    case OutputTypePdf:
        surface = cairo_pdf_surface_create_for_stream(writeCairo, (void *)this->out_file, surface_width * ppi, (surface_height)*ppi); //默认单位是mm，所以需要mm转inch
        //cairo_surface_set_fallback_resolution(surface,300,300);//设置分辨率
        cr = cairo_create(surface);                //创建画笔
        cairo_scale(cr, scale * ppi, scale * ppi); //缩放画笔，因PDF用mm作为最终单位故需缩放画笔
        break;

    case OutputTypeSvg:
        surface = cairo_svg_surface_create_for_stream(writeCairo, (void *)this->out_file, surface_width * ppi, surface_height * ppi); //默认单位pt
        cr = cairo_create(surface);                                                                                                   //创建画笔
        cairo_scale(cr, scale * ppi, scale * ppi);
        break;

    case OutputTypePng:
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, surface_width * ppi, surface_height * ppi); //默认单位pt
        cr = cairo_create(surface);                                                                           //创建画笔
        cairo_scale(cr, scale * ppi, scale * ppi);
        break;

    default:
        assert("Renderer::Renderer: error: Unknow type , Failure to initialize!\n");
        break;
    }
}

Renderer::~Renderer()
{
    if (surface_type == OutputTypePng)
        cairo_surface_write_to_png_stream(surface, writeCairo, (void *)this->out_file);

    cairo_destroy(cr);              //回收画笔
    cairo_surface_destroy(surface); //回收介质

    fclose(this->out_file);
}

// void Renderer::nextpage()
// {
//     switch (surface_type)
//     {
//     case outputTypePdf:
//         cairo_show_page(cr);
//         break;
//     case outputTypeSvg:
//         fprintf(stderr, "Renderer::NextPage: warning: SVG surface not support multi-page,!\n");
//         break;
//     case outputTypePng:
//         fprintf(stderr, "Renderer::NextPage: warning: PNG surface not support multi-page,!\n");
//         break;
//     default:
//         fprintf(stderr, "Renderer::NextPage: error: Unknow type , Failure to initialize!\n");
//         break;
//     }
// }

cairo_status_t writeCairo(void *closure, const unsigned char *data, unsigned int length)
{
    fwrite(data, length, 1, (FILE *)closure);
    return CAIRO_STATUS_SUCCESS;
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
    if (!this->filecheck(svgfilename.c_str()))
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
    if (!this->filecheck(pngfilename.c_str()))
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

int8_t Renderer::filecheck(const char *filename)
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
