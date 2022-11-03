#include <ArtRobot/ArtRobot.hpp>

int main(int argc, char *argv[])
{
    // fromFile
    {
        auto img = ArtRobot::Component::Image::fromFile("img", 0, 0, 512, 512, 0, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72); // 渲染png
        renderer.render(img->getSurface());
        renderer.saveToFile("TestImage-Result-fromFile.png");
    }

    // fromFileByCV
    {
        auto img = ArtRobot::Component::Image::fromFileByCV("img", 0, 0, 512, 512, 0, "img.jpg");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72); // 渲染png
        renderer.render(img->getSurface());
        renderer.saveToFile("TestImage-Result-fromFileByCV.png");
    }

    return 0;
}
