#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <cstdlib>

class Image
{
private:
    uint32_t width;
    uint32_t height;
    uint8_t *buffer;

public:
    Image(uint32_t width, uint32_t height);
    ~Image();

    uint32_t getWidth();
    uint32_t getHeight();
    uint8_t *getBuffer();
};

#endif // IMAGE_H