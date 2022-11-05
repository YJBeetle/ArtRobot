#include <ArtRobot/ArtRobot.hpp>

int main(int argc, char *argv[]) {
    // Rectangle
    {
        auto c = ArtRobot::Component::Rectangle("Rectangle", 100, 200, {.x=256, .y=256}, "red");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Rectangle.png");
    }
    // RectangleRound
    {
        auto c = ArtRobot::Component::RectangleRound("RectangleRound", 100, 200, {.x=256, .y=256}, 10, 20, 30, 40, "red");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-RectangleRound.png");
    }
    // Circle
    {
        auto c = ArtRobot::Component::Circle("Circle", 100, 200, {.x=256, .y=256}, "red");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Circle.png");
    }

    return 0;
}
