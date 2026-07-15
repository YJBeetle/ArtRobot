#pragma once

#include <ArtRobot/ArtRobot.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace TestSupport {
    struct Bounds {
        int left = 0;
        int top = 0;
        int right = -1;
        int bottom = -1;

        bool empty() const {
            return right < left || bottom < top;
        }
    };

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

    inline void expectColor(Context &test, uint32_t argb,
                            uint8_t expectedRed, uint8_t expectedGreen, uint8_t expectedBlue,
                            uint8_t tolerance, const std::string &message) {
        test.expect(alpha(argb) == 0xff &&
                    std::abs(static_cast<int>(red(argb)) - expectedRed) <= tolerance &&
                    std::abs(static_cast<int>(green(argb)) - expectedGreen) <= tolerance &&
                    std::abs(static_cast<int>(blue(argb)) - expectedBlue) <= tolerance,
                    message);
    }

    inline bool startsWith(const std::vector<unsigned char> &data,
                           const std::vector<unsigned char> &signature) {
        return data.size() >= signature.size() &&
               std::equal(signature.begin(), signature.end(), data.begin());
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

    inline Bounds opaqueBounds(const std::vector<unsigned char> &data,
                               int width, int height) {
        Bounds bounds{width, height, -1, -1};
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (alpha(pixelAt(data, width, x, y)) == 0)
                    continue;
                bounds.left = std::min(bounds.left, x);
                bounds.top = std::min(bounds.top, y);
                bounds.right = std::max(bounds.right, x);
                bounds.bottom = std::max(bounds.bottom, y);
            }
        }
        return bounds;
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
