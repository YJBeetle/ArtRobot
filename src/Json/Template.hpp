/*
 * Copyright 2026 YJBeetle
 *
 * This work is licensed under the terms of the GNU GPL, version 3.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <ArtRobot/Component/Base.hpp>
#include <ArtRobot/Renderer.hpp>

namespace ArtRobot::Json {

using ResourceLoader = std::function<std::vector<uint8_t>(const std::string &source)>;

struct Document {
    double width = 200;
    double height = 200;
    Unit unit = Unit::Pixel;
    double ppi = 72;
    std::shared_ptr<Component::Base> body;
};

Document parseTemplate(const std::vector<uint8_t> &json,
                       ResourceLoader resourceLoader = {});

Document parseTemplate(const std::string &json,
                       ResourceLoader resourceLoader = {});

} // namespace ArtRobot::Json
