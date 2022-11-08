#include <fstream>
#include <ArtRobot/ArtRobot.hpp>

int main(int argc, char *argv[]) {
    // fromPng
    {
        auto img = ArtRobot::Component::Image::fromPng("img", {.x=256, .y=256}, "img.png", 200);
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
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
            auto img = ArtRobot::Component::Image::fromPng("img", {.x=256, .y=256}, d, 0, 200);
            ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
            renderer.render(img.getSurface());
            renderer.saveToFile("TestImage-Result-fromPngBinary.png");
        }
    }

    // fromJpg
    {
        auto img = ArtRobot::Component::Image::fromJpg("img", {.x=256, .y=256}, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
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
            auto img = ArtRobot::Component::Image::fromJpg("img", {.x=256, .y=256}, d);
            ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
            renderer.render(img.getSurface());
            renderer.saveToFile("TestImage-Result-fromJpgBinary.png");
        }
    }

    // JpegGray
    {
        auto img = ArtRobot::Component::Image::fromJpg("img", {.x=256, .y=256}, "img-Gray.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-JpegGray.png");
    }

    // JpegCmyk
    {
        auto img = ArtRobot::Component::Image::fromJpg("img", {.x=256, .y=256}, "img-CMYK.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-JpegCmyk.png");
    }

#ifdef OpenCV_FOUND
#ifndef OpenCV_WITHOUT_IMAPI
    // fromFileByCV
    {
        auto img = ArtRobot::Component::Image::fromFileByCV("img", {.x=256, .y=256}, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromFileByCV.png");
    }
#endif
#endif

    return 0;
}
