#ifndef JPEG_H
#define JPEG_H

#include "default.h"

#include "Image.h"

void write_JPEG_file(const char *filename, int quality, shared_ptr<Image> image);
shared_ptr<Image> read_JPEG_file(const char *filename);

#endif // JPEG_H