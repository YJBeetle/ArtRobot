#include "TestSupport.hpp"

#include <exception>
#include <string>
#include <vector>

int main() {
    TestSupport::Context test;

    auto expectImageError = [&test](auto source, const std::string &message) {
        try {
            auto image = ArtRobot::Component::Image("invalid", {}, source);
            test.expect(false, message);
        } catch (const std::exception &) {
        }
    };

    expectImageError(std::vector<uint8_t>{}, "Empty image data must fail");
    expectImageError(std::vector<uint8_t>{0x89, 0x50}, "Short image signature must fail");
    expectImageError(std::vector<uint8_t>{0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A},
                     "Corrupt PNG data must fail");
    expectImageError(std::vector<uint8_t>{0xFF, 0xD8, 0xFF}, "Corrupt JPEG data must fail");
    expectImageError(std::string("missing.png"), "Missing image file must fail");
    expectImageError(std::string("unsupported.webp"), "Missing WebP file must fail");

    std::vector<uint8_t> pngData;
    test.expect(TestSupport::readBinaryFile("img.png", pngData), "Read PNG fixture");
    if (!pngData.empty()) {
        auto fileImage = ArtRobot::Component::Image("png-file", {.x=256, .y=256}, "img.png");
        auto memoryImage = ArtRobot::Component::Image("png-memory", {.x=256, .y=256}, pngData);
        const auto filePixels = TestSupport::renderPixmap(fileImage, 512, 512);
        const auto memoryPixels = TestSupport::renderPixmap(memoryImage, 512, 512);
        test.expect(filePixels == memoryPixels, "PNG file and memory decoding must match");
        test.expect(TestSupport::opaquePixelCount(filePixels, 512, 512) > 0, "PNG output must contain pixels");
    }

    std::vector<uint8_t> jpegData;
    test.expect(TestSupport::readBinaryFile("img.jpg", jpegData), "Read JPEG fixture");
    if (!jpegData.empty()) {
        auto fileImage = ArtRobot::Component::Image("jpeg-file", {.x=256, .y=256}, "img.jpg");
        auto memoryImage = ArtRobot::Component::Image("jpeg-memory", {.x=256, .y=256}, jpegData);
        const auto filePixels = TestSupport::renderPixmap(fileImage, 512, 512);
        const auto memoryPixels = TestSupport::renderPixmap(memoryImage, 512, 512);
        test.expect(filePixels == memoryPixels, "JPEG file and memory decoding must match");
        test.expect(TestSupport::opaquePixelCount(filePixels, 512, 512) > 0, "JPEG output must contain pixels");
    }

    {
        auto image = ArtRobot::Component::Image("gray", {.x=256, .y=256}, "img-Gray.jpg");
        const auto pixels = TestSupport::renderPixmap(image, 512, 512);
        const auto center = TestSupport::pixelAt(pixels, 512, 256, 256);
        test.expect(TestSupport::alpha(center) == 0xff, "Gray JPEG center must be opaque");
        test.expect(TestSupport::red(center) == TestSupport::green(center) &&
                    TestSupport::green(center) == TestSupport::blue(center),
                    "Gray JPEG channels must match");
    }

    {
        auto image = ArtRobot::Component::Image("cmyk", {.x=256, .y=256}, "img-CMYK.jpg");
        const auto pixels = TestSupport::renderPixmap(image, 512, 512);
        test.expect(TestSupport::opaquePixelCount(pixels, 512, 512) > 0,
                    "CMYK JPEG output must contain pixels");
        test.expect(TestSupport::alpha(TestSupport::pixelAt(pixels, 512, 256, 256)) == 0xff,
                    "CMYK JPEG center must be opaque");
    }

#ifdef OpenCV_FOUND
#ifndef OpenCV_WITHOUT_IMAPI
    {
        auto imageMat = cv::imread("img.png", cv::IMREAD_UNCHANGED);
        test.expect(!imageMat.empty(), "OpenCV must read PNG fixture");
        if (!imageMat.empty()) {
            auto image = ArtRobot::Component::Image("opencv", {.x=256, .y=256}, imageMat);
            test.expect(TestSupport::opaquePixelCount(TestSupport::renderPixmap(image, 512, 512), 512, 512) > 0,
                        "OpenCV image output must contain pixels");
        }
    }
#endif
#endif

#ifdef WEBP_FOUND
    {
        auto source = ArtRobot::Component::Rectangle(
                "source", {.anchor=ArtRobot::Transform::LT}, 160, 120, ArtRobot::Color::Lime);
        ArtRobot::Renderer webpRenderer(ArtRobot::OutputType::Webp, 160, 120);
        webpRenderer.render(source.getSurface());
        const auto webpData = webpRenderer.getData();
        webpRenderer.saveToFile("TestImage-Source.webp");

        auto memoryImage = ArtRobot::Component::Image(
                "webp-memory", {.anchor=ArtRobot::Transform::LT}, webpData);
        auto fileImage = ArtRobot::Component::Image(
                "webp-file", {.anchor=ArtRobot::Transform::LT}, "TestImage-Source.webp");
        const auto memoryPixels = TestSupport::renderPixmap(memoryImage, 160, 120);
        const auto filePixels = TestSupport::renderPixmap(fileImage, 160, 120);
        test.expect(memoryPixels == filePixels, "WebP file and memory decoding must match");
        const auto center = TestSupport::pixelAt(memoryPixels, 160, 80, 60);
        test.expect(TestSupport::red(center) <= 4 &&
                    TestSupport::green(center) >= 251 &&
                    TestSupport::blue(center) <= 4,
                    "WebP round trip color");
    }
#endif

    return test.result();
}
