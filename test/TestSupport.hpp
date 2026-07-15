#pragma once

#include <ArtRobot/ArtRobot.hpp>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace TestSupport {
    class Context {
    public:
        void expect(bool condition, const std::string &message) {
            if (!condition) {
                std::cerr << "FAILED: " << message << '\n';
                ++failures;
            }
        }

        int result() const {
            return failures == 0 ? 0 : 1;
        }

    private:
        int failures = 0;
    };

    inline std::vector<unsigned char> renderPixmap(const ArtRobot::Component::Base &component,
                                                    int width, int height) {
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Pixmap, width, height);
        renderer.render(component.getSurface());
        return renderer.getData();
    }

    inline uint32_t pixelAt(const std::vector<unsigned char> &data,
                            int width, int x, int y) {
        const auto stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
        uint32_t pixel = 0;
        std::memcpy(&pixel, data.data() + y * stride + x * 4, sizeof(pixel));
        return pixel;
    }

    inline uint8_t alpha(uint32_t argb) {
        return (argb >> 24) & 0xff;
    }

    inline uint8_t red(uint32_t argb) {
        return (argb >> 16) & 0xff;
    }

    inline uint8_t green(uint32_t argb) {
        return (argb >> 8) & 0xff;
    }

    inline uint8_t blue(uint32_t argb) {
        return argb & 0xff;
    }

    inline size_t opaquePixelCount(const std::vector<unsigned char> &data,
                                   int width, int height) {
        size_t count = 0;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (alpha(pixelAt(data, width, x, y)) != 0)
                    ++count;
            }
        }
        return count;
    }

    inline bool readBinaryFile(const std::string &path, std::vector<uint8_t> &data) {
        std::ifstream input(path, std::ios::binary | std::ios::ate);
        if (!input)
            return false;
        const auto size = input.tellg();
        if (size <= 0)
            return false;
        data.resize(static_cast<size_t>(size));
        input.seekg(0);
        return static_cast<bool>(input.read(reinterpret_cast<char *>(data.data()), size));
    }
}
