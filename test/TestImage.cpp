#include <fstream>
#include <iostream>
#include <ArtRobot/ArtRobot.hpp>

int main(int argc, char *argv[]) {
    auto expectImageError = [](auto source) {
        try {
            auto image = ArtRobot::Component::Image("invalid", {}, source);
        } catch (const std::exception &) {
            return true;
        }
        return false;
    };

    if (!expectImageError(std::vector<uint8_t>{}) ||
        !expectImageError(std::vector<uint8_t>{0x89, 0x50}) ||
        !expectImageError(std::vector<uint8_t>{0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}) ||
        !expectImageError(std::vector<uint8_t>{0xFF, 0xD8, 0xFF}) ||
        !expectImageError(std::string("missing.png")) ||
        !expectImageError(std::string("unsupported.webp")))
        return 1;

    // fromPng
    {
        auto img = ArtRobot::Component::Image("img", {.x=256, .y=256}, "img.png", 200);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromPng.png");
    }

    // fromPngBinary
    {
        std::ifstream is("img.png", std::ios::binary | std::ios::ate);
        if (!is.is_open())
            std::cout << "failed to open file" << '\n';
        else {
            std::vector<uint8_t> d(is.tellg());
            is.seekg(0);
            is.read(reinterpret_cast<char *>(d.data()), d.size());
            auto img = ArtRobot::Component::Image("img", {.x=256, .y=256}, d, NAN, 200);
            ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
            renderer.render(img.getSurface());
            renderer.saveToFile("TestImage-Result-fromPngBinary.png");
        }
    }

    // fromJpg
    {
        auto img = ArtRobot::Component::Image("img", {.x=256, .y=256}, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromJpg.png");
    }

    // fromJpgBinary
    {
        std::ifstream is("img.jpg", std::ios::binary | std::ios::ate);
        if (!is.is_open())
            std::cout << "failed to open file" << '\n';
        else {
            std::vector<uint8_t> d(is.tellg());
            is.seekg(0);
            is.read(reinterpret_cast<char *>(d.data()), d.size());
            auto img = ArtRobot::Component::Image("img", {.x=256, .y=256}, d);
            ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
            renderer.render(img.getSurface());
            renderer.saveToFile("TestImage-Result-fromJpgBinary.png");
        }
    }

    // JpegGray
    {
        auto img = ArtRobot::Component::Image("img", {.x=256, .y=256}, "img-Gray.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-JpegGray.png");
    }

    // JpegCmyk
    {
        auto img = ArtRobot::Component::Image("img", {.x=256, .y=256}, "img-CMYK.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-JpegCmyk.png");
    }

#ifdef OpenCV_FOUND
#ifndef OpenCV_WITHOUT_IMAPI
    // fromFileByCV
    {
        auto imgMat = cv::imread("img.png", cv::IMREAD_UNCHANGED);
        auto img = ArtRobot::Component::Image("img", {.x=256, .y=256}, imgMat);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromFileByCV.png");
    }
#endif
#endif

#ifdef WEBP_FOUND
    // WebP output and input round trip
    {
        auto source = ArtRobot::Component::Rectangle("source", {.anchor=ArtRobot::Transform::LT}, 160, 120, ArtRobot::Color::Lime);
        ArtRobot::Renderer webpRenderer(ArtRobot::OutputType::Webp, 160, 120);
        webpRenderer.render(source.getSurface());
        const auto webpData = webpRenderer.getData();
        webpRenderer.saveToFile("TestImage-Source.webp");

        auto memoryImage = ArtRobot::Component::Image("webp-memory", {.anchor=ArtRobot::Transform::LT}, webpData);
        ArtRobot::Renderer memoryRenderer(ArtRobot::OutputType::Png, 160, 120);
        memoryRenderer.render(memoryImage.getSurface());
        memoryRenderer.saveToFile("TestImage-Result-fromWebpBinary.png");

        auto fileImage = ArtRobot::Component::Image("webp-file", {.anchor=ArtRobot::Transform::LT}, "TestImage-Source.webp");
        ArtRobot::Renderer fileRenderer(ArtRobot::OutputType::Png, 160, 120);
        fileRenderer.render(fileImage.getSurface());
        fileRenderer.saveToFile("TestImage-Result-fromWebp.png");
    }
#endif

    return 0;
}
