#include "./Args.hpp"

#include <ArtRobot/ArtRobot.hpp>
#include <ArtRobot/Json/Template.hpp>

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

std::vector<uint8_t> readFile(const std::filesystem::path &path) {
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input)
        throw std::runtime_error("Failed to open resource: " + path.string());

    const auto size = input.tellg();
    if (size <= 0)
        throw std::runtime_error("Resource is empty: " + path.string());

    std::vector<uint8_t> data(static_cast<size_t>(size));
    input.seekg(0);
    if (!input.read(reinterpret_cast<char *>(data.data()), size))
        throw std::runtime_error("Failed to read resource: " + path.string());
    return data;
}

} // namespace

int main(int argc, char *argv[]) {
    const auto startedAt = std::chrono::steady_clock::now();

    try {
        Args args(argc, argv);
        const auto templatePath = std::filesystem::absolute(args.jsonfile);
        const auto resourceDirectory = templatePath.parent_path();
        const auto document = ArtRobot::Json::parseTemplate(
            readFile(templatePath),
            [resourceDirectory](const std::string &source) {
                auto resourcePath = std::filesystem::path(source);
                if (resourcePath.is_relative())
                    resourcePath = resourceDirectory / resourcePath;
                return readFile(resourcePath);
            });

        ArtRobot::Renderer renderer(args.type,
                                    document.width,
                                    document.height,
                                    document.unit,
                                    document.ppi);
        renderer.render(document.body->getSurface());
        renderer.saveToFile(args.output);

#ifdef TIMER
        const auto elapsed = std::chrono::duration<double>(
            std::chrono::steady_clock::now() - startedAt);
        std::cerr << "Used Time: " << elapsed.count() << "s\n";
#endif
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "ArtRobotRender: " << error.what() << '\n';
        return 1;
    }
}
