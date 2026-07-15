#include "TestSupport.hpp"

#include <cstring>
#include <string>
#include <vector>

namespace {
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

    void expectRasterSize(TestSupport::Context &test,
                          double width, double height,
                          ArtRobot::Unit unit, double ppi,
                          int expectedWidth, int expectedHeight,
                          const std::string &message) {
        auto rectangle = ArtRobot::Component::Rectangle(
                "Rectangle", {.anchor=ArtRobot::Transform::LT}, 1, 1, ArtRobot::Color::Black);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Pixmap, width, height, unit, ppi);
        renderer.render(rectangle.getSurface());
        const auto expectedSize = static_cast<size_t>(
                cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, expectedWidth) * expectedHeight);
        test.expect(renderer.getDataCSize() == expectedSize, message);
    }

    void expectRoundTrip(TestSupport::Context &test,
                         const std::vector<unsigned char> &encoded,
                         uint8_t red, uint8_t green, uint8_t blue,
                         uint8_t tolerance, const std::string &message) {
        auto image = ArtRobot::Component::Image(
                "Image", {.anchor=ArtRobot::Transform::LT}, encoded);
        const auto pixels = TestSupport::renderPixmap(image, 32, 24);
        TestSupport::expectColor(test, TestSupport::pixelAt(pixels, 32, 16, 12),
                                 red, green, blue, tolerance, message);
    }
}

int main() {
    TestSupport::Context test;

    const auto pixmap = renderRectangle(ArtRobot::OutputType::Pixmap, ArtRobot::Color::Red);
    test.expect(pixmap.size() == static_cast<size_t>(cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, 32) * 24),
                "Pixmap byte length");
    TestSupport::expectColor(test, TestSupport::pixelAt(pixmap, 32, 16, 12),
                             255, 0, 0, 0, "Pixmap center pixel");

    const auto png = renderRectangle(ArtRobot::OutputType::Png, ArtRobot::Color::Aqua);
    test.expect(TestSupport::startsWith(png, {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a}),
                "PNG signature");
    expectRoundTrip(test, png, 0, 255, 255, 0, "PNG round trip color");

    const auto pdf = renderRectangle(ArtRobot::OutputType::Pdf, ArtRobot::Color::Black);
    test.expect(TestSupport::startsWith(pdf, {'%', 'P', 'D', 'F', '-'}), "PDF signature");

    const auto svg = renderRectangle(ArtRobot::OutputType::Svg, ArtRobot::Color::Lime);
    const std::string svgText(svg.begin(), svg.end());
    test.expect(svgText.find("<svg") != std::string::npos, "SVG document element");

#ifdef JPEG_FOUND
    const auto jpeg = renderRectangle(ArtRobot::OutputType::Jpeg, ArtRobot::Color::Red);
    test.expect(TestSupport::startsWith(jpeg, {0xff, 0xd8, 0xff}), "JPEG signature");
    expectRoundTrip(test, jpeg, 255, 0, 0, 4, "JPEG round trip color");
#endif

#ifdef WEBP_FOUND
    const auto webp = renderRectangle(ArtRobot::OutputType::Webp, ArtRobot::Color::Lime);
    test.expect(webp.size() >= 12 &&
                std::memcmp(webp.data(), "RIFF", 4) == 0 &&
                std::memcmp(webp.data() + 8, "WEBP", 4) == 0,
                "WebP container signature");
    expectRoundTrip(test, webp, 0, 255, 0, 4, "WebP round trip color");
#endif

    std::vector<uint32_t> rawPixels(4, 0xff0000ff);
    auto rawImage = ArtRobot::Component::Image(
            "Raw", {.anchor=ArtRobot::Transform::LT},
            reinterpret_cast<unsigned char *>(rawPixels.data()),
            2, 2, 2 * 4, true);
    const auto rawOutput = TestSupport::renderPixmap(rawImage, 2, 2);
    TestSupport::expectColor(test, TestSupport::pixelAt(rawOutput, 2, 1, 1),
                             0, 0, 255, 0, "Raw BGRA input color");

    expectRasterSize(test, 37, 19, ArtRobot::Unit::Pixel, 96, 37, 19, "Pixel dimensions");
    expectRasterSize(test, 1, 0.5, ArtRobot::Unit::Inch, 72, 72, 36, "Inch conversion");
    expectRasterSize(test, 25.4, 12.7, ArtRobot::Unit::Millimeter, 72, 72, 36, "Millimeter conversion");
    expectRasterSize(test, 2.54, 1.27, ArtRobot::Unit::Centimeter, 72, 72, 36, "Centimeter conversion");

    return test.result();
}
