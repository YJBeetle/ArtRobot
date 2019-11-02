
#include <setjmp.h>

#include "Jpeg.h"

#include <jpeglib.h>

struct my_error_mgr
{
    struct jpeg_error_mgr pub; /* "public" fields */
    jmp_buf setjmp_buffer;     /* for return to caller */
};

typedef struct my_error_mgr *my_error_ptr;

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr)cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

void write_JPEG_file(const char *filename, int quality, shared_ptr<Image> image)
{
    //get
    JSAMPLE *image_buffer = image->getBuffer();
    uint32_t image_width = image->getWidth();
    uint32_t image_height = image->getHeight();

    /* More stuff */
    FILE *outfile;           /* target file */
    JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
    int row_stride;          /* physical row width in image buffer */

    struct jpeg_compress_struct cinfo;

    //出错处理
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);
    if ((outfile = fopen(filename, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = image_width; /* image width and height, in
                         * pixels */
    cinfo.image_height = image_height;
    cinfo.input_components = 3;     /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; /* colorspace of input image */

    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG
                                  values */
    );

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = image_width * 3; /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}

shared_ptr<Image> read_JPEG_file(const char *filename)
{
    FILE *infile;      /* source file */
    JSAMPARRAY buffer; /* Output row buffer */
    int row_stride;    /* physical row width in output buffer */

    //文件检查
    if ((infile = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "文件不存在： %s \n", filename);
        return make_shared<Image>(0, 0);
    }

    struct jpeg_decompress_struct cinfo;

    //出错处理
    struct my_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return make_shared<Image>(0, 0);
    }

    //准备
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void)jpeg_read_header(&cinfo, TRUE);
    (void)jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    //获取参数申请内存
    auto image = make_shared<Image>(cinfo.output_width, cinfo.output_height);
    uint8_t *image_buffer = image->getBuffer();

    //开始读取
    while (cinfo.output_scanline < cinfo.output_height)
    {
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(image_buffer, buffer[0], row_stride);
        for (int i = 0; i < cinfo.output_width; i++)
        {
            image_buffer[i * 4 + 1] = buffer[0][i * cinfo.output_components + 0];
            image_buffer[i * 4 + 2] = buffer[0][i * cinfo.output_components + 1];
            image_buffer[i * 4 + 3] = buffer[0][i * cinfo.output_components + 2];
        }
        image_buffer += cinfo.output_width * 4;
        // put_scanline_someplace(buffer[0], row_stride);
    }

    //清理
    (void)jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    return image;
}