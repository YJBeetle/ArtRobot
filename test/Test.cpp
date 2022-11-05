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
    // Scale
    {
        auto c = ArtRobot::Component::RectangleRound("RectangleRound", 200, 200, {.x=256, .y=256, .scaleX = .5, .scaleY=2.}, 10, 20, 30, 40, "red");
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 512, 512, ArtRobot::Renderer::PX, 72);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Scale.png");
    }
    // Anchor
    {
        ArtRobot::Component::Circle cl[]={
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::LT}, "red"),
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::CT}, "green"),
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::RT}, "blue"),
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::LC}, "yellow"),
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::CC}, "purple"),
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::RC}, "lime"),
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::LD}, "teal"),
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::CD}, "aqua"),
                ArtRobot::Component::Circle("Circle", 200, 200, {.anchor=ArtRobot::Component::Transform::Anchor::RD}, "maroon"),
        };
        auto g = ArtRobot::Component::Group("Group", {.x=200, .y=200});
        for (const auto &c: cl)
            g.addChild(c);
        ArtRobot::Renderer renderer(ArtRobot::OutputTypePng, 400, 400, ArtRobot::Renderer::PX, 72);
        renderer.render(g.getSurface());
        renderer.saveToFile("Test-Result-Anchor.png");
    }

    return 0;
}
