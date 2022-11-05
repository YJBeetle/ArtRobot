#include <ArtRobot/ArtRobot.hpp>

int main(int argc, char *argv[]) {
    // Circle
    {
        auto c = ArtRobot::Component::Circle("Circle", 100, 200, {.x=0, .y=512}, "red");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Circle.png");
    }

    return 0;
}
