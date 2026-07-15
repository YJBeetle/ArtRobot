#include "TestSupport.hpp"

#include <array>
#include <cmath>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

namespace {
    void expectColor(TestSupport::Context &test, uint32_t argb,
                     uint8_t red, uint8_t green, uint8_t blue,
                     const std::string &message) {
        test.expect(TestSupport::alpha(argb) == 0xff &&
                    TestSupport::red(argb) == red &&
                    TestSupport::green(argb) == green &&
                    TestSupport::blue(argb) == blue,
                    message);
    }

    void expectTransparent(TestSupport::Context &test, uint32_t argb,
                           const std::string &message) {
        test.expect(TestSupport::alpha(argb) == 0, message);
    }

    void expectBounds(TestSupport::Context &test,
                      const TestSupport::Bounds &actual,
                      const TestSupport::Bounds &expected,
                      int tolerance,
                      const std::string &message) {
        test.expect(!actual.empty() &&
                    std::abs(actual.left - expected.left) <= tolerance &&
                    std::abs(actual.top - expected.top) <= tolerance &&
                    std::abs(actual.right - expected.right) <= tolerance &&
                    std::abs(actual.bottom - expected.bottom) <= tolerance,
                    message);
    }

    void save(const ArtRobot::Component::Base &component,
              ArtRobot::OutputType type,
              int width, int height,
              const std::string &filename) {
        ArtRobot::Renderer renderer(type, width, height);
        renderer.render(component.getSurface());
        renderer.saveToFile(filename);
    }
}

int main() {
    TestSupport::Context test;

    // Rectangle
    {
        auto component = ArtRobot::Component::Rectangle(
                "Rectangle", {.x=256, .y=256}, 100, 200, ArtRobot::Color::Red);
        const auto pixels = TestSupport::renderPixmap(component, 512, 512);
        expectColor(test, TestSupport::pixelAt(pixels, 512, 256, 256), 255, 0, 0,
                    "Rectangle center color");
        expectTransparent(test, TestSupport::pixelAt(pixels, 512, 100, 100),
                          "Rectangle outside must be transparent");
        expectBounds(test, TestSupport::opaqueBounds(pixels, 512, 512), {206, 156, 305, 355}, 1,
                     "Rectangle bounds");
        save(component, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Component-Rectangle.png");
    }

    // RectangleRound
    {
        auto component = ArtRobot::Component::RectangleRound(
                "RectangleRound", {.x=256, .y=256}, 100, 200,
                10, 20, 30, 40, ArtRobot::Color::Red);
        const auto pixels = TestSupport::renderPixmap(component, 512, 512);
        expectColor(test, TestSupport::pixelAt(pixels, 512, 256, 256), 255, 0, 0,
                    "Rounded rectangle center color");
        expectTransparent(test, TestSupport::pixelAt(pixels, 512, 206, 156),
                          "Rounded rectangle corner must be transparent");
        save(component, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Component-RectangleRound.png");
    }

    // Circle
    {
        auto component = ArtRobot::Component::Circle(
                "Circle", {.x=256, .y=256}, 100, 200, ArtRobot::Color::Red);
        const auto pixels = TestSupport::renderPixmap(component, 512, 512);
        expectColor(test, TestSupport::pixelAt(pixels, 512, 256, 256), 255, 0, 0,
                    "Circle center color");
        expectTransparent(test, TestSupport::pixelAt(pixels, 512, 206, 156),
                          "Circle corner must be transparent");
        save(component, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Component-Circle.png");
    }

    // Mask
    {
        auto maskShape = ArtRobot::Component::Circle(
                "Circle", {.x=256, .y=256}, 200, 200, ArtRobot::Color::White);
        auto image = ArtRobot::Component::Image("Image", {.x=256, .y=256}, "img.jpg");
        auto mask = ArtRobot::Component::Mask("Mask", 512, 512, {.x=256, .y=256}, maskShape, image);
        const auto pixels = TestSupport::renderPixmap(mask, 512, 512);
        test.expect(TestSupport::alpha(TestSupport::pixelAt(pixels, 512, 256, 256)) != 0,
                    "Mask center must contain image data");
        expectTransparent(test, TestSupport::pixelAt(pixels, 512, 100, 100),
                          "Mask outside must be transparent");
        save(mask, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Component-Mask.png");
    }

    // Mask ownership and null input
    {
        auto maskShape = std::make_shared<ArtRobot::Component::Circle>(
                "Circle", ArtRobot::Transform{.x=128, .y=128}, 160, 160, ArtRobot::Color::White);
        auto child = std::make_shared<ArtRobot::Component::Rectangle>(
                "Rectangle", ArtRobot::Transform{.anchor=ArtRobot::Transform::LT},
                256, 256, ArtRobot::Color::Fuchsia);
        auto mask = ArtRobot::Component::Mask(
                "OwnedMask", 256, 256, {.x=128, .y=128}, maskShape, child);
        maskShape.reset();
        child.reset();
        const auto pixels = TestSupport::renderPixmap(mask, 256, 256);
        expectColor(test, TestSupport::pixelAt(pixels, 256, 128, 128), 255, 0, 255,
                    "Owned mask center color");
        expectTransparent(test, TestSupport::pixelAt(pixels, 256, 0, 0),
                          "Owned mask outside must be transparent");

        try {
            auto invalidMask = ArtRobot::Component::Mask(
                    "Invalid", 10, 10, {}, maskShape, mask);
            test.expect(false, "Null mask component must fail");
        } catch (const std::invalid_argument &) {
        }
        save(mask, ArtRobot::OutputType::Png, 256, 256, "Test-Result-Component-MaskOwnership.png");
    }

    // Text
    {
        auto component = ArtRobot::Component::Text(
                "Text", {.x=256, .y=256}, "喵喵喵", ArtRobot::Color::Black,
                "", 900, 100, ArtRobot::HorizontalAlign::Center, ArtRobot::VerticalAlign::Center);
        const auto pixels = TestSupport::renderPixmap(component, 512, 512);
        test.expect(TestSupport::opaquePixelCount(pixels, 512, 512) > 100,
                    "Text must produce visible glyphs");
        save(component, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Component-Text.png");
    }

    // TextArea
    {
        auto component = ArtRobot::Component::TextArea(
                "TextArea", {.x=256, .y=256}, 512, 512,
                "喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵",
                ArtRobot::Color::Black, "", 900, 100);
        const auto pixels = TestSupport::renderPixmap(component, 512, 512);
        test.expect(TestSupport::opaquePixelCount(pixels, 512, 512) > 500,
                    "TextArea must produce visible glyphs");
        save(component, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Component-TextArea.png");
    }

    // Group
    {
        auto first = ArtRobot::Component::Rectangle(
                "Rectangle1", {.rotate=20}, 100, 100, ArtRobot::Color::Red);
        auto nested = ArtRobot::Component::Group("Group1", {.rotate=20, .scaleX=.5});
        nested.addChild(first);
        auto second = ArtRobot::Component::Rectangle(
                "Rectangle2", {.rotate=20}, 100, 100, ArtRobot::Color::Blue);
        auto group = ArtRobot::Component::Group("Group2", {.x=256, .y=256});
        group.addChild(second);
        group.addChild(nested);
        const auto pixels = TestSupport::renderPixmap(group, 512, 512);
        test.expect(TestSupport::opaquePixelCount(pixels, 512, 512) > 1000,
                    "Nested group must produce visible pixels");
        save(group, ArtRobot::OutputType::Pdf, 512, 512, "Test-Result-Component-Group.pdf");
    }

    // Scale
    {
        auto component = ArtRobot::Component::RectangleRound(
                "RectangleRound", {.x=256, .y=256, .scaleX=.5, .scaleY=2.},
                200, 200, 10, 20, 30, 40, ArtRobot::Color::Red);
        const auto pixels = TestSupport::renderPixmap(component, 512, 512);
        expectBounds(test, TestSupport::opaqueBounds(pixels, 512, 512), {206, 56, 305, 455}, 2,
                     "Scaled component bounds");
        save(component, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Transform-Scale.png");
    }

    // Anchors
    {
        const std::array<ArtRobot::Transform::Anchor, 9> anchors = {
                ArtRobot::Transform::LT, ArtRobot::Transform::CT, ArtRobot::Transform::RT,
                ArtRobot::Transform::LC, ArtRobot::Transform::CC, ArtRobot::Transform::RC,
                ArtRobot::Transform::LD, ArtRobot::Transform::CD, ArtRobot::Transform::RD,
        };
        const std::array<TestSupport::Bounds, 9> expected = {
                TestSupport::Bounds{100, 100, 139, 129}, TestSupport::Bounds{80, 100, 119, 129}, TestSupport::Bounds{60, 100, 99, 129},
                TestSupport::Bounds{100, 85, 139, 114}, TestSupport::Bounds{80, 85, 119, 114}, TestSupport::Bounds{60, 85, 99, 114},
                TestSupport::Bounds{100, 70, 139, 99}, TestSupport::Bounds{80, 70, 119, 99}, TestSupport::Bounds{60, 70, 99, 99},
        };
        for (size_t index = 0; index < anchors.size(); ++index) {
            auto component = ArtRobot::Component::Circle(
                    "Circle", {.x=100, .y=100, .anchor=anchors[index]},
                    40, 30, ArtRobot::Color::Red);
            expectBounds(test,
                         TestSupport::opaqueBounds(TestSupport::renderPixmap(component, 200, 200), 200, 200),
                         expected[index], 1, "Anchor bounds " + std::to_string(index));
        }
    }

    // Repeat
    {
        auto tile = ArtRobot::Component::Circle(
                "Tile", {.anchor=ArtRobot::Transform::LT}, 40, 30, ArtRobot::Color::Aqua);
        auto repeat = ArtRobot::Component::Repeat("Repeat", {.x=256, .y=256}, 200, 120);
        repeat.addChild(tile);
        const auto pixels = TestSupport::renderPixmap(repeat, 512, 512);
        expectColor(test, TestSupport::pixelAt(pixels, 512, 176, 211), 0, 255, 255,
                    "Repeat first tile center");
        expectColor(test, TestSupport::pixelAt(pixels, 512, 336, 301), 0, 255, 255,
                    "Repeat last tile center");
        expectTransparent(test, TestSupport::pixelAt(pixels, 512, 155, 211),
                          "Repeat must clip outside its left edge");
        save(repeat, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Component-Repeat.png");
    }

#ifdef JPEG_FOUND
    // JPEG output
    {
        auto background = ArtRobot::Component::Rectangle(
                "Background", {.anchor=ArtRobot::Transform::LT},
                320, 200, ArtRobot::Color::Red);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Jpeg, 320, 200);
        renderer.render(background.getSurface());
        const auto data = renderer.getData();
        test.expect(data.size() >= 3 && data[0] == 0xff && data[1] == 0xd8 && data[2] == 0xff,
                    "JPEG output signature");
        renderer.saveToFile("Test-Result-Renderer-Jpeg.jpg");
    }
#endif

#ifdef RSVG_FOUND
    // SVG
    {
        unsigned char data[] = R"(<svg xmlns="http://www.w3.org/2000/svg" width="120" height="80"><rect width="120" height="80" fill="#00ffff"/></svg>)";
        auto component = ArtRobot::Component::Svg(
                "Svg", {.x=256, .y=256}, 240, 160,
                data, std::strlen(reinterpret_cast<char *>(data)));
        const auto pixels = TestSupport::renderPixmap(component, 512, 512);
        expectColor(test, TestSupport::pixelAt(pixels, 512, 256, 256), 0, 255, 255,
                    "SVG center color");
        expectTransparent(test, TestSupport::pixelAt(pixels, 512, 100, 100),
                          "SVG outside must be transparent");

        unsigned char invalid[] = "not svg";
        try {
            auto invalidSvg = ArtRobot::Component::Svg(
                    "Invalid", {}, 100, 100, invalid, sizeof(invalid) - 1);
            test.expect(false, "Invalid SVG data must fail");
        } catch (const std::exception &) {
        }
        save(component, ArtRobot::OutputType::Png, 512, 512, "Test-Result-Component-Svg.png");
    }
#endif

    return test.result();
}
