/*
 * Copyright 2022 YJBeetle
 *
 * Authors:
 *  YJBeetle <YJBeetle@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 3.  See
 * the LICENSE file in the top-level directory.
 *
 */

#include "./Image.hpp"

#include <csetjmp>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

#ifdef JPEG_FOUND

#include <jpeglib.h>

#endif

#ifdef WEBP_FOUND

#include <webp/decode.h>

#endif

namespace ArtRobot {
    namespace Component {

        namespace {
            constexpr size_t DdsHeaderSize = 148;
            constexpr uint32_t DdsHeaderFlags = 0x100f;
            constexpr uint32_t DdsPixelFormatFourCc = 0x4;
            constexpr uint32_t DdsCapsTexture = 0x1000;
            constexpr uint32_t DxgiFormatB8G8R8A8Unorm = 87;
            constexpr uint32_t DdsDimensionTexture2d = 3;
            constexpr uint32_t DdsAlphaModePremultiplied = 2;
            constexpr uint32_t DdsAlphaModeOpaque = 3;

            uint32_t readLittleEndian32(const std::vector<uint8_t> &data, size_t offset) {
                if (offset > data.size() || data.size() - offset < sizeof(uint32_t))
                    throw std::invalid_argument("Truncated DDS header");
                return static_cast<uint32_t>(data[offset]) |
                       (static_cast<uint32_t>(data[offset + 1]) << 8) |
                       (static_cast<uint32_t>(data[offset + 2]) << 16) |
                       (static_cast<uint32_t>(data[offset + 3]) << 24);
            }

            std::vector<uint8_t> readBinaryFile(const std::string &filename) {
                std::ifstream input(filename, std::ios::binary | std::ios::ate);
                if (!input)
                    return {};
                const auto size = input.tellg();
                if (size <= 0)
                    return {};
                std::vector<uint8_t> data(static_cast<size_t>(size));
                input.seekg(0);
                if (!input.read(reinterpret_cast<char *>(data.data()), size))
                    return {};
                return data;
            }
        }

        Image::Image(std::string name)
                : Base({name}, {}) {
        }

        Image::Image(std::string name, Transform transform,
                     unsigned char *imageData,
                     int imageColums, int imageRows,
                     int imageStride, bool isPremultiplied,
                     double width, double height)
                : Image(name, transform, surfaceFromRaw(imageData, imageColums, imageRows, imageStride, isPremultiplied), width, height) {
        }

        Image::Image(std::string name, Transform transform,
                     const std::vector<uint8_t> &data,
                     double width, double height)
                : Image(name, transform, validateSurface(surfaceFromFile(data)), width, height) {
        }

        Image::Image(std::string name, Transform transform,
                     const std::string &filename,
                     double width, double height)
                : Image(name, transform, validateSurface(surfaceFromFile(filename)), width, height) {
        }

#ifdef OpenCV_FOUND

        Image::Image(std::string name, Transform transform,
                     const cv::Mat &imageMat,
                     double width, double height)
                : Image(name, transform, surfaceFromRaw(imageMat.data, imageMat.cols, imageMat.rows, imageMat.step, false), width, height) {
        }

#endif

        Image::~Image() {
            if (imageSurface)
                cairo_surface_destroy(imageSurface);
        }

        Image::Image(std::string name, Transform transform,
                     cairo_surface_t *_imageSurface,
                     double width, double height)
                : Base({name,
                        !std::isnan(width) ? width : !std::isnan(height) ? (height / cairo_image_surface_get_height(_imageSurface) * cairo_image_surface_get_width(_imageSurface)) : cairo_image_surface_get_width(_imageSurface),
                        !std::isnan(height) ? height : !std::isnan(width) ? (width / cairo_image_surface_get_width(_imageSurface) * cairo_image_surface_get_height(_imageSurface)) : cairo_image_surface_get_height(_imageSurface)},
                       transform),
                  imageSurface(_imageSurface),
                  imageCols(cairo_image_surface_get_width(imageSurface)),
                  imageRows(cairo_image_surface_get_height(imageSurface)) {
            if (!std::isnan(width) || !std::isnan(height))
                cairo_scale(cr,
                            property.width / imageCols,
                            property.height / imageRows);
            cairo_set_source_surface(cr, imageSurface, 0, 0);
            cairo_paint(cr);
        }

        cairo_surface_t *Image::validateSurface(cairo_surface_t *surface) {
            if (!surface)
                throw std::invalid_argument("Unsupported image source");
            const auto status = cairo_surface_status(surface);
            if (status != CAIRO_STATUS_SUCCESS) {
                const std::string message = cairo_status_to_string(status);
                cairo_surface_destroy(surface);
                throw std::runtime_error("Failed to decode image: " + message);
            }
            return surface;
        }

        cairo_surface_t *Image::surfaceFromRaw(unsigned char *imageData,
                                               int imageCols, int imageRows,
                                               int imageStride, bool isPremultiplied) {
            if (!imageData || imageCols <= 0 || imageRows <= 0 ||
                imageStride < cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, imageCols))
                throw std::invalid_argument("Invalid raw image buffer");

            // 计算预乘
            if (!isPremultiplied) {
                // 尝试 cairo_set_operator CAIRO_OPERATOR_OVER CAIRO_OPERATOR_SOURCE ?
                for (int y = 0; y < imageRows; y++)
                    for (int x = 0; x < imageCols; x++) {
                        auto p = imageData + y * imageStride + x * 4;
                        p[0] = (unsigned short) p[0] * p[3] / 0xff;
                        p[1] = (unsigned short) p[1] * p[3] / 0xff;
                        p[2] = (unsigned short) p[2] * p[3] / 0xff;
                    }
            }

            return cairo_image_surface_create_for_data(imageData,
                                                       CAIRO_FORMAT_ARGB32,
                                                       imageCols,
                                                       imageRows,
                                                       imageStride);
        }

        cairo_surface_t *Image::surfaceFromPng(const std::vector<uint8_t> &data) {
            struct PngStreamClosure {
                const uint8_t *data;
                const size_t max_size;
                size_t pos;
            } pngStreamClosure{data.data(), data.size(), 0};
            return cairo_image_surface_create_from_png_stream(
                    [](void *_closure,
                       unsigned char *data,
                       unsigned int length) -> cairo_status_t {
                        PngStreamClosure *closure = (PngStreamClosure *) _closure;
                        if ((closure->pos + length) > (closure->max_size))
                            return CAIRO_STATUS_READ_ERROR;
                        memcpy(data, (closure->data + closure->pos), length);
                        closure->pos += length;
                        return CAIRO_STATUS_SUCCESS;
                    }, (void *) &pngStreamClosure);
        }

        cairo_surface_t *Image::surfaceFromPng(const std::string &filename) {
            if (std::filesystem::exists(filename))
                return cairo_image_surface_create_from_png(filename.c_str());
            return nullptr;
        }

        cairo_surface_t *Image::surfaceFromDds(const std::vector<uint8_t> &data) {
            static const uint8_t ddsSignature[] = {'D', 'D', 'S', ' '};
            static const uint8_t dx10FourCc[] = {'D', 'X', '1', '0'};
            if (data.size() < DdsHeaderSize ||
                memcmp(data.data(), ddsSignature, sizeof(ddsSignature)) != 0)
                return nullptr;

            if (readLittleEndian32(data, 4) != 124 ||
                readLittleEndian32(data, 76) != 32)
                throw std::invalid_argument("Invalid DDS header size");
            if ((readLittleEndian32(data, 8) & DdsHeaderFlags) != DdsHeaderFlags ||
                (readLittleEndian32(data, 80) & DdsPixelFormatFourCc) == 0 ||
                memcmp(data.data() + 84, dx10FourCc, sizeof(dx10FourCc)) != 0 ||
                (readLittleEndian32(data, 108) & DdsCapsTexture) == 0)
                throw std::invalid_argument("Unsupported DDS header flags");

            const auto height = readLittleEndian32(data, 12);
            const auto width = readLittleEndian32(data, 16);
            const auto pitch = readLittleEndian32(data, 20);
            const auto depth = readLittleEndian32(data, 24);
            const auto mipmapCount = readLittleEndian32(data, 28);
            const auto caps2 = readLittleEndian32(data, 112);
            const auto dxgiFormat = readLittleEndian32(data, 128);
            const auto resourceDimension = readLittleEndian32(data, 132);
            const auto miscFlag = readLittleEndian32(data, 136);
            const auto arraySize = readLittleEndian32(data, 140);
            const auto alphaMode = readLittleEndian32(data, 144) & 0x7;

            if (width == 0 || height == 0 ||
                width > static_cast<uint32_t>(std::numeric_limits<int>::max()) ||
                height > static_cast<uint32_t>(std::numeric_limits<int>::max()))
                throw std::invalid_argument("Invalid DDS dimensions");
            if (depth != 0 || mipmapCount > 1 || caps2 != 0 || miscFlag != 0 ||
                arraySize != 1 || resourceDimension != DdsDimensionTexture2d ||
                dxgiFormat != DxgiFormatB8G8R8A8Unorm ||
                (alphaMode != DdsAlphaModePremultiplied && alphaMode != DdsAlphaModeOpaque))
                throw std::invalid_argument("Unsupported DDS texture layout");

            const auto cairoStride = cairo_format_stride_for_width(
                    CAIRO_FORMAT_ARGB32, static_cast<int>(width));
            if (cairoStride < 0 || pitch < static_cast<uint32_t>(cairoStride) ||
                pitch > static_cast<uint32_t>(std::numeric_limits<int>::max()))
                throw std::invalid_argument("Invalid DDS row pitch");
            if (height > (std::numeric_limits<size_t>::max() - DdsHeaderSize) / pitch)
                throw std::invalid_argument("DDS payload size overflows");
            const auto payloadSize = static_cast<size_t>(pitch) * height;
            if (data.size() != DdsHeaderSize + payloadSize)
                throw std::invalid_argument("Invalid DDS payload size");

            auto *surface = cairo_image_surface_create(
                    CAIRO_FORMAT_ARGB32, static_cast<int>(width), static_cast<int>(height));
            if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
                return surface;
            auto *destination = cairo_image_surface_get_data(surface);
            for (uint32_t row = 0; row < height; ++row) {
                memcpy(destination + static_cast<size_t>(row) * cairoStride,
                       data.data() + DdsHeaderSize + static_cast<size_t>(row) * pitch,
                       static_cast<size_t>(cairoStride));
            }
            cairo_surface_mark_dirty(surface);
            return surface;
        }

        cairo_surface_t *Image::surfaceFromDds(const std::string &filename) {
            const auto data = readBinaryFile(filename);
            return data.empty() ? nullptr : surfaceFromDds(data);
        }

#ifdef WEBP_FOUND

        cairo_surface_t *Image::surfaceFromWebp(const std::vector<uint8_t> &data) {
            int width = 0;
            int height = 0;
            if (!WebPGetInfo(data.data(), data.size(), &width, &height))
                return nullptr;

            auto *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
            if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
                return surface;

            auto *surfaceData = cairo_image_surface_get_data(surface);
            const auto stride = cairo_image_surface_get_stride(surface);
            if (!WebPDecodeBGRAInto(data.data(), data.size(), surfaceData, stride * height, stride)) {
                cairo_surface_destroy(surface);
                return nullptr;
            }
            cairo_surface_mark_dirty(surface);
            return surface;
        }

        cairo_surface_t *Image::surfaceFromWebp(const std::string &filename) {
            std::ifstream input(filename, std::ios::binary | std::ios::ate);
            if (!input)
                return nullptr;
            const auto size = input.tellg();
            if (size <= 0)
                return nullptr;
            std::vector<uint8_t> data(static_cast<size_t>(size));
            input.seekg(0);
            if (!input.read(reinterpret_cast<char *>(data.data()), size))
                return nullptr;
            return surfaceFromWebp(data);
        }

#endif

#ifdef JPEG_FOUND

        class JpegReader {
        private:
            struct ErrorManager {
                jpeg_error_mgr base;
                std::jmp_buf jumpBuffer;
                char message[JMSG_LENGTH_MAX]{};
            } errorManager;

            jpeg_decompress_struct cInfo;
            cairo_surface_t *imageSurface = nullptr;

        public:
            JpegReader() {
                cInfo.err = jpeg_std_error(&errorManager.base);
                errorManager.base.error_exit = [](j_common_ptr cInfo) {
                    auto *manager = reinterpret_cast<ErrorManager *>(cInfo->err);
                    (*cInfo->err->format_message)(cInfo, manager->message);
                    std::longjmp(manager->jumpBuffer, 1);
                };
                jpeg_create_decompress(&cInfo);
            }

            ~JpegReader() {
                if (imageSurface)
                    cairo_surface_destroy(imageSurface);
                jpeg_destroy_decompress(&cInfo);
            }

            void loadFromMem(const unsigned char *inBuffer, unsigned long inSize) {
                jpeg_mem_src(&cInfo, inBuffer, inSize);
            }

            void loadFromStdio(FILE *inFile) {
                jpeg_stdio_src(&cInfo, inFile);
            }

            cairo_surface_t *read() {
                if (setjmp(errorManager.jumpBuffer))
                    throw std::runtime_error("Failed to decode JPEG: " + std::string(errorManager.message));

                (void) jpeg_read_header(&cInfo, true);
                (void) jpeg_start_decompress(&cInfo);

                int row_stride = cInfo.output_width * cInfo.output_components;
                JSAMPARRAY buffer = (*cInfo.mem->alloc_sarray)((j_common_ptr) &cInfo, JPOOL_IMAGE, row_stride, 1);

                imageSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, cInfo.output_width, cInfo.output_height);
                auto imageSurfaceData = cairo_image_surface_get_data(imageSurface);
                auto imageSurfaceStride = cairo_image_surface_get_stride(imageSurface);

                if (cInfo.output_components == 1) {
                    while (cInfo.output_scanline < cInfo.output_height) {
                        (void) jpeg_read_scanlines(&cInfo, buffer, 1);
                        for (int col = 0; col < cInfo.output_width; col++) {
                            size_t ofsI = col * 4;
                            imageSurfaceData[ofsI + 0] = imageSurfaceData[ofsI + 1] = imageSurfaceData[ofsI + 2] = buffer[0][col];
                        }
                        imageSurfaceData += imageSurfaceStride;
                    }
                } else if (cInfo.output_components == 3 || cInfo.output_components == 4) {
                    while (cInfo.output_scanline < cInfo.output_height) {
                        (void) jpeg_read_scanlines(&cInfo, buffer, 1);
                        for (int col = 0; col < cInfo.output_width; col++) {
                            size_t ofsI = col * 4;
                            size_t ofsJ = col * cInfo.output_components;
                            imageSurfaceData[ofsI + 0] = buffer[0][ofsJ + 2];
                            imageSurfaceData[ofsI + 1] = buffer[0][ofsJ + 1];
                            imageSurfaceData[ofsI + 2] = buffer[0][ofsJ + 0];
                        }
                        imageSurfaceData += imageSurfaceStride;
                    }
                }

                (void) jpeg_finish_decompress(&cInfo);
                auto *result = imageSurface;
                imageSurface = nullptr;
                return result;
            }
        };

        cairo_surface_t *Image::surfaceFromJpg(const std::vector<uint8_t> &data) {
            JpegReader r;
            r.loadFromMem(data.data(), data.size());
            return r.read();
        }

        cairo_surface_t *Image::surfaceFromJpg(const std::string &filename) {
            FILE *inFile;
            if ((inFile = fopen(filename.c_str(), "rb")) == nullptr)
                return nullptr;
            JpegReader r;
            r.loadFromStdio(inFile);
            auto rr = r.read();
            fclose(inFile);
            return rr;
        }

#endif

        cairo_surface_t *Image::surfaceFromFile(const std::vector<uint8_t> &data) {
            static const uint8_t pngSignature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
            static const uint8_t jpegSignature[] = {0xFF, 0xD8, 0xFF};
            static const uint8_t ddsSignature[] = {'D', 'D', 'S', ' '};
            static const uint8_t riffSignature[] = {'R', 'I', 'F', 'F'};
            static const uint8_t webpSignature[] = {'W', 'E', 'B', 'P'};
            if (data.size() >= sizeof(pngSignature) && memcmp(data.data(), pngSignature, sizeof(pngSignature)) == 0)
                return surfaceFromPng(data);
            else if (data.size() >= sizeof(ddsSignature) && memcmp(data.data(), ddsSignature, sizeof(ddsSignature)) == 0)
                return surfaceFromDds(data);
#ifdef WEBP_FOUND
            else if (data.size() >= 12 &&
                     memcmp(data.data(), riffSignature, sizeof(riffSignature)) == 0 &&
                     memcmp(data.data() + 8, webpSignature, sizeof(webpSignature)) == 0)
                return surfaceFromWebp(data);
#endif
#ifdef JPEG_FOUND
            else if (data.size() >= sizeof(jpegSignature) && memcmp(data.data(), jpegSignature, sizeof(jpegSignature)) == 0)
                return surfaceFromJpg(data);
#endif
            return nullptr;
        }

        cairo_surface_t *Image::surfaceFromFile(const std::string &filename) {
            const auto extension = std::filesystem::path(filename).extension().string();
            if (!strcasecmp(extension.c_str(), ".png"))
                return surfaceFromPng(filename);
            else if (!strcasecmp(extension.c_str(), ".dds"))
                return surfaceFromDds(filename);
#ifdef WEBP_FOUND
            else if (!strcasecmp(extension.c_str(), ".webp"))
                return surfaceFromWebp(filename);
#endif
#ifdef JPEG_FOUND
            else if (!strcasecmp(extension.c_str(), ".jpg") || !strcasecmp(extension.c_str(), ".jpeg"))
                return surfaceFromJpg(filename);
#endif
            return nullptr;
        }

    } // namespace Component
} // namespace ArtRobot
