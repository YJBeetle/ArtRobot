#include <fstream>
#include <ArtRobot/ArtRobot.hpp>

int main(int argc, char *argv[]) {
    // fromPng
    {
        auto img = ArtRobot::Component::Image::fromPng("img", {.x=256, .y=256}, "img.png");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromPng.png");
    }

    // fromJpg
    {
        auto img = ArtRobot::Component::Image::fromJpg("img", {.x=256, .y=256}, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromJpg.png");
    }

#ifdef OpenCV_FOUND
    // fromFileByCV
    {
        auto img = ArtRobot::Component::Image::fromFileByCV("img", {.x=256, .y=256}, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromFileByCV.png");
    }
#endif

    return 0;
}
