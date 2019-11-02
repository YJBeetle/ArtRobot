#include "Image.h"

Image::Image(uint32_t __width, uint32_t __height) : width(__width),
                                                    height(__height)
{
    buffer = (uint8_t *)malloc(width * height * 4);
}

Image::~Image()
{
    free(buffer);
}

uint32_t Image::getWidth()
{
    return width;
}
uint32_t Image::getHeight()
{
    return height;
}
uint8_t *Image::getBuffer()
{
    return buffer;
}