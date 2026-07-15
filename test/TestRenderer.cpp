#include <ArtRobot/ArtRobot.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace {
    int failures = 0;

    void expect(bool condition, const std::string &message) {
        if (!condition) {
            std::cerr << "FAILED: " << message << '\n';
            ++failures;
        }
    }

    bool startsWith(const std::vector<unsigned char> &data,
                    const std::vector<unsigned char> &signature) {
        return data.size() >= signature.size() &&
               std::equal(signature.begin(), signature.end(), data.begin());
    }

    uint32_t pixelAt(const std::vector<unsigned char> &data,
                     int width, int x, int y) {
        const auto stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
        uint32_t pixel = 0;
        std::memcpy(&pixel, data.data() + y * stride + x * 4, sizeof(pixel));
        return pixel;
    }

    void expectColor(uint32_t argb, uint8_t red, uint8_t green, uint8_t blue,
                     uint8_t tolerance, const std::string &message) {
        const auto actualRed = static_cast<int>((argb >> 16) & 0xff);
        const auto actualGreen = static_cast<int>((argb >> 8) & 0xff);
        const auto actualBlue = static_cast<int>(argb & 0xff);
        expect(std::abs(actualRed - red) <= tolerance &&
               std::abs(actualGreen - green) <= tolerance &&
               std::abs(actualBlue - blue) <= tolerance,
               message);
    }

    std::vector<unsigned char> renderRectangle(ArtRobot::OutputType type,
                                                ArtRobot::Color color,
                                                int width = 32,
                                                int height = 24) {
        auto rectangle = ArtRobot::Component::Rectangle(
                "Rectangle", {.anchor=ArtRobot::Transform::LT}, width, height, color);
        ArtRobot::Renderer renderer(type, width, height);
        renderer.render(rectangle.getSurface());
        return renderer.getData();
    }

    void expectRasterSize(double width, double height,
                          ArtRobot::Unit unit, double ppi,
                          int expectedWidth, int expectedHeight,
                          const std::string &message) {
        auto rectangle = ArtRobot::Component::Rectangle(
                "Rectangle", {.anchor=ArtRobot::Transform::LT}, 1, 1, ArtRobot::Color::Black);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Pixmap, width, height, unit, ppi);
        renderer.render(rectangle.getSurface());
        const auto expectedSize = static_cast<size_t>(
                cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, expectedWidth) * expectedHeight);
        expect(renderer.getDataCSize() == expectedSize, message);
    }

    void expectRoundTrip(const std::vector<unsigned char> &encoded,
                         uint8_t red, uint8_t green, uint8_t blue,
                         uint8_t tolerance, const std::string &message) {
        auto image = ArtRobot::Component::Image(
                "Image", {.anchor=ArtRobot::Transform::LT}, encoded);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Pixmap, 32, 24);
        renderer.render(image.getSurface());
        expectColor(pixelAt(renderer.getData(), 32, 16, 12),
                    red, green, blue, tolerance, message);
    }
}

int main() {
    const auto pixmap = renderRectangle(ArtRobot::OutputType::Pixmap, ArtRobot::Color::Red);
    expect(pixmap.size() == static_cast<size_t>(cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, 32) * 24),
           "Pixmap byte length");
    expectColor(pixelAt(pixmap, 32, 16, 12), 255, 0, 0, 0, "Pixmap center pixel");

    const auto png = renderRectangle(ArtRobot::OutputType::Png, ArtRobot::Color::Aqua);
    expect(startsWith(png, {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a}),
           "PNG signature");
    expectRoundTrip(png, 0, 255, 255, 0, "PNG round trip color");

    const auto pdf = renderRectangle(ArtRobot::OutputType::Pdf, ArtRobot::Color::Black);
    expect(startsWith(pdf, {'%', 'P', 'D', 'F', '-'}), "PDF signature");

    const auto svg = renderRectangle(ArtRobot::OutputType::Svg, ArtRobot::Color::Lime);
    const std::string svgText(svg.begin(), svg.end());
    expect(svgText.find("<svg") != std::string::npos, "SVG document element");

#ifdef JPEG_FOUND
    const auto jpeg = renderRectangle(ArtRobot::OutputType::Jpeg, ArtRobot::Color::Red);
    expect(startsWith(jpeg, {0xff, 0xd8, 0xff}), "JPEG signature");
    expectRoundTrip(jpeg, 255, 0, 0, 4, "JPEG round trip color");
#endif

#ifdef WEBP_FOUND
    const auto webp = renderRectangle(ArtRobot::OutputType::Webp, ArtRobot::Color::Lime);
    expect(webp.size() >= 12 &&
           std::memcmp(webp.data(), "RIFF", 4) == 0 &&
           std::memcmp(webp.data() + 8, "WEBP", 4) == 0,
           "WebP container signature");
    expectRoundTrip(webp, 0, 255, 0, 4, "WebP round trip color");
#endif

    std::vector<uint32_t> rawPixels(4, 0xff0000ff);
    auto rawImage = ArtRobot::Component::Image(
            "Raw", {.anchor=ArtRobot::Transform::LT},
            reinterpret_cast<unsigned char *>(rawPixels.data()),
            2, 2, 2 * 4, true);
    ArtRobot::Renderer rawRenderer(ArtRobot::OutputType::Pixmap, 2, 2);
    rawRenderer.render(rawImage.getSurface());
    expectColor(pixelAt(rawRenderer.getData(), 2, 1, 1), 0, 0, 255, 0, "Raw BGRA input color");

    expectRasterSize(37, 19, ArtRobot::Unit::Pixel, 96, 37, 19, "Pixel dimensions");
    expectRasterSize(1, 0.5, ArtRobot::Unit::Inch, 72, 72, 36, "Inch conversion");
    expectRasterSize(25.4, 12.7, ArtRobot::Unit::Millimeter, 72, 72, 36, "Millimeter conversion");
    expectRasterSize(2.54, 1.27, ArtRobot::Unit::Centimeter, 72, 72, 36, "Centimeter conversion");

    return failures == 0 ? 0 : 1;
}
