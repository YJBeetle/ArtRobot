#include <ArtRobot/ArtRobot.hpp>

int main(int argc, char *argv[]) {
    // Rectangle
    {
        auto c = ArtRobot::Component::Rectangle("Rectangle", {.x=256, .y=256}, 100, 200, ArtRobot::Color::Red);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Component-Rectangle.png");
    }
    // RectangleRound
    {
        auto c = ArtRobot::Component::RectangleRound("RectangleRound", {.x=256, .y=256}, 100, 200, 10, 20, 30, 40, ArtRobot::Color::Red);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Component-RectangleRound.png");
    }
    // Circle
    {
        auto c = ArtRobot::Component::Circle("Circle", {.x=256, .y=256}, 100, 200, ArtRobot::Color::Red);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Component-Circle.png");
    }
    // Mask
    {
        auto mask = ArtRobot::Component::Circle("Circle", {.x=256, .y=256}, 200, 200, ArtRobot::Color::Red);
        auto img = ArtRobot::Component::Image::fromJpg("Img", {.x=256, .y=256}, "img.jpg");
        auto c = ArtRobot::Component::Mask("Body", 512, 512, {.x=256, .y=256}, mask, img);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Component-Mask.png");
    }
    // Text
    {
        auto c = ArtRobot::Component::Text("Text", {.x=256, .y=256}, "喵喵喵", ArtRobot::Color::Black, "", 900, 100, ArtRobot::HorizontalAlign::Center, ArtRobot::VerticalAlign::Center);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Component-Text.png");
    }
    // TextArea
    {
        auto c = ArtRobot::Component::TextArea("TextArea", {.x=256, .y=256}, 512, 512, "喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵喵", ArtRobot::Color::Black, "", 900, 100);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Component-TextArea.png");
    }
    // Group
    {
        auto c1 = ArtRobot::Component::Rectangle("Rectangle1", {.rotate=20}, 100, 100, ArtRobot::Color::Red);
        auto g1 = ArtRobot::Component::Group("Group1", {.rotate=20, .scaleX=.5});
        g1.addChild(c1);
        auto c2 = ArtRobot::Component::Rectangle("Rectangle2", {.rotate=20}, 100, 100, ArtRobot::Color::Blue);
        auto g2 = ArtRobot::Component::Group("Group2", {.x=256, .y=256});
        g2.addChild(c2);
        g2.addChild(g1);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Pdf, 512, 512);
        renderer.render(g2.getSurface());
        renderer.saveToFile("Test-Result-Component-Group.pdf");
    }
    // Scale
    {
        auto c = ArtRobot::Component::RectangleRound("RectangleRound", {.x=256, .y=256, .scaleX = .5, .scaleY=2.}, 200, 200, 10, 20, 30, 40, ArtRobot::Color::Red);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 512, 512);
        renderer.render(c.getSurface());
        renderer.saveToFile("Test-Result-Transform-Scale.png");
    }
    // Anchor
    {
        ArtRobot::Component::Circle cl[] = {
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::LT}, 200, 200, ArtRobot::Color::Red),
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::CT}, 200, 200, ArtRobot::Color::Lime),
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::RT}, 200, 200, ArtRobot::Color::Blue),
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::LC}, 200, 200, ArtRobot::Color::Yellow),
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::CC}, 200, 200, ArtRobot::Color::Aqua),
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::RC}, 200, 200, ArtRobot::Color::Fuchsia),
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::LD}, 200, 200, ArtRobot::Color::Purple),
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::CD}, 200, 200, ArtRobot::Color::Teal),
                ArtRobot::Component::Circle("Circle", {.anchor=ArtRobot::Transform::Anchor::RD}, 200, 200, ArtRobot::Color::Navy),
        };
        auto g = ArtRobot::Component::Group("Group", {.x=200, .y=200});
        for (const auto &c: cl)
            g.addChild(c);
        ArtRobot::Renderer renderer(ArtRobot::OutputType::Png, 400, 400);
        renderer.render(g.getSurface());
        renderer.saveToFile("Test-Result-Transform-Anchor.png");
    }

    return 0;
}
