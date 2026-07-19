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

#ifdef PANGO_FOUND
    const std::string measuredLayout = R"({
        "w": {"op": "sub", "args": [120, {"var": "unusedWidth"}]},
        "h": {"op": "sub", "args": [220, {"var": "unusedHeight"}]},
        "layout": {
            "variables": {
                "unusedWidth": {
                    "op": "max",
                    "args": [
                        0,
                        {
                            "op": "sub",
                            "args": [
                                100,
                                {
                                    "op": "max",
                                    "args": [
                                        {"measure": "message.realW"},
                                        {"measure": "sender.realW"}
                                    ]
                                }
                            ]
                        }
                    ]
                },
                "unusedHeight": {
                    "op": "max",
                    "args": [
                        0,
                        {
                            "op": "sub",
                            "args": [200, {"measure": "message.realH"}]
                        }
                    ]
                }
            }
        },
        "body": {
            "type": "group",
            "child": [
                {
                    "type": "rectangle",
                    "w": {"op": "sub", "args": [120, {"var": "unusedWidth"}]},
                    "h": {"op": "sub", "args": [220, {"var": "unusedHeight"}]},
                    "anchor": 0,
                    "color": "#00ff00"
                },
                {
                    "type": "text",
                    "name": "sender",
                    "content": "Sender",
                    "fontFamily": "sans",
                    "fontSize": 20,
                    "maxWidth": 100,
                    "anchor": 0
                },
                {
                    "type": "textArea",
                    "name": "message",
                    "content": "Measured text",
                    "fontFamily": "sans",
                    "fontSize": 20,
                    "w": 100,
                    "h": 200,
                    "anchor": 0
                }
            ]
        }
    })";
    const auto measuredDocument = ArtRobot::Json::parseTemplate(measuredLayout);
    test.expect(measuredDocument.width > 20 && measuredDocument.width <= 120,
                "Measured layout width");
    test.expect(measuredDocument.height > 20 && measuredDocument.height < 220,
                "Measured layout height");
    const auto measuredPixels = TestSupport::renderPixmap(
        *measuredDocument.body,
        static_cast<int>(std::ceil(measuredDocument.width)),
        static_cast<int>(std::ceil(measuredDocument.height)));
    test.expect(TestSupport::opaquePixelCount(
                    measuredPixels,
                    static_cast<int>(std::ceil(measuredDocument.width)),
                    static_cast<int>(std::ceil(measuredDocument.height))) > 100,
                "Measured layout renders");

    expectThrows(test, R"({
        "layout":{"variables":{"a":{"var":"b"},"b":{"var":"a"}}},
        "w":{"var":"a"},
        "body":{"type":"rectangle"}
    })", "Layout variable cycles must fail");
    expectThrows(test, R"({
        "layout":{},
        "w":{"measure":"missing.realW"},
        "body":{"type":"rectangle"}
    })", "Unknown measurements must fail");
#endif

    expectThrows(test, R"({"body":{"type":"unknown"}})",
                 "Unknown component must fail");
    expectThrows(test, R"({"w":0,"body":{"type":"rectangle"}})",
                 "Invalid canvas must fail");
    expectThrows(test, R"({"body":{"type":"image","src":"missing.png"}})",
                 "Image without loader must fail");

    return test.result();
}
