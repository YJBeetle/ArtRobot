#include <ArtRobot/ArtRobot.hpp>

int main(int argc, char *argv[]) {
    // fromFile
    {
        auto img = ArtRobot::Component::Image::fromFile("img", 512, 512, {.x=256, .y=256}, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72); // 渲染png
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromFile.png");
    }

#ifdef OpenCV_FOUND
    // fromFileByCV
    {
        auto img = ArtRobot::Component::Image::fromFileByCV("img", 512, 512, {.x=256, .y=256}, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72); // 渲染png
        renderer.render(img.getSurface());
        renderer.saveToFile("TestImage-Result-fromFileByCV.png");
    }
#endif

    return 0;
}
