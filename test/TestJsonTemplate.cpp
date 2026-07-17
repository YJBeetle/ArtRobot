#include "TestSupport.hpp"

#include <ArtRobot/Json/Template.hpp>

#include <stdexcept>
#include <string>
#include <vector>

namespace {

void expectThrows(TestSupport::Context &test,
                  const std::string &json,
                  const std::string &message) {
    try {
        ArtRobot::Json::parseTemplate(json);
        test.expect(false, message);
    } catch (const std::exception &) {
    }
}

} // namespace

int main() {
    TestSupport::Context test;

    const std::string shapes = R"({
        "w": 32,
        "h": 24,
        "unit": "px",
        "ppi": 96,
        "inputs": {"ignored": {"type": "color"}},
        "body": {
            "type": "group",
            "name": "body",
            "child": [
                {
                    "type": "rectangle",
                    "name": "background",
                    "w": 32,
                    "h": 24,
                    "anchor": 0,
                    "color": "#ff0000",
                    "bindings": {"color": "ignored"}
                }
            ]
        }
    })";

    const auto shapeDocument = ArtRobot::Json::parseTemplate(shapes);
    test.expect(shapeDocument.width == 32, "Template width");
    test.expect(shapeDocument.height == 24, "Template height");
    test.expect(shapeDocument.ppi == 96, "Template PPI");
    test.expect(shapeDocument.unit == ArtRobot::Unit::Pixel, "Template unit");
    const auto pixels = TestSupport::renderPixmap(*shapeDocument.body, 32, 24);
    TestSupport::expectColor(test, TestSupport::pixelAt(pixels, 32, 16, 12),
                             255, 0, 0, 0, "Parsed rectangle color");

    std::vector<uint8_t> image;
    test.expect(TestSupport::readBinaryFile("img.png", image), "Read image fixture");
    int loadCount = 0;
    const std::string images = R"({
        "w": 64,
        "h": 48,
        "body": {
            "type": "group",
            "child": [
                {"type": "image", "src": "fixture.png", "w": 32, "h": 24, "anchor": 0},
                {"type": "image", "src": "fixture.png", "x": 32, "w": 32, "h": 24, "anchor": 0}
            ]
        }
    })";
    const auto imageDocument = ArtRobot::Json::parseTemplate(
        images,
        [&](const std::string &source) {
            test.expect(source == "fixture.png", "Resource loader source");
            ++loadCount;
            return image;
        });
    test.expect(loadCount == 1, "Resources are loaded once per source");
    test.expect(TestSupport::opaquePixelCount(
                    TestSupport::renderPixmap(*imageDocument.body, 64, 48), 64, 48) > 100,
                "Parsed image produces pixels");

    expectThrows(test, R"({"body":{"type":"unknown"}})",
                 "Unknown component must fail");
    expectThrows(test, R"({"w":0,"body":{"type":"rectangle"}})",
                 "Invalid canvas must fail");
    expectThrows(test, R"({"body":{"type":"image","src":"missing.png"}})",
                 "Image without loader must fail");

    return test.result();
}
