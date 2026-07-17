/*
 * Copyright 2026 YJBeetle
 *
 * This work is licensed under the terms of the GNU GPL, version 3.
 */

#include "Template.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include <ArtRobot/ArtRobot.hpp>
#include <ArtRobot/Types/TextAlign.hpp>
#include <nlohmann/json.hpp>

namespace ArtRobot::Json {
namespace {

using NlohmannJson = nlohmann::json;
using ComponentPtr = std::shared_ptr<Component::Base>;

std::string fieldPath(const std::string &path, const std::string &field) {
    return path + "." + field;
}

std::string lowercase(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}

const NlohmannJson *findField(const NlohmannJson &object, const char *field) {
    const auto iterator = object.find(field);
    return iterator == object.end() ? nullptr : &*iterator;
}

double numberOr(const NlohmannJson &object,
                const char *field,
                double fallback,
                const std::string &path) {
    const auto *value = findField(object, field);
    if (!value || value->is_null())
        return fallback;
    if (!value->is_number())
        throw std::invalid_argument(fieldPath(path, field) + " must be a number");
    const auto number = value->get<double>();
    if (!std::isfinite(number))
        throw std::invalid_argument(fieldPath(path, field) + " must be finite");
    return number;
}

int integerOr(const NlohmannJson &object,
              const char *field,
              int fallback,
              const std::string &path) {
    const auto *value = findField(object, field);
    if (!value || value->is_null())
        return fallback;
    if (!value->is_number_integer())
        throw std::invalid_argument(fieldPath(path, field) + " must be an integer");
    return value->get<int>();
}

std::string stringOr(const NlohmannJson &object,
                     const char *field,
                     std::string fallback,
                     const std::string &path) {
    const auto *value = findField(object, field);
    if (!value || value->is_null())
        return fallback;
    if (!value->is_string())
        throw std::invalid_argument(fieldPath(path, field) + " must be a string");
    return value->get<std::string>();
}

std::string requiredString(const NlohmannJson &object,
                           const char *field,
                           const std::string &path) {
    const auto value = stringOr(object, field, {}, path);
    if (value.empty())
        throw std::invalid_argument(fieldPath(path, field) + " must not be empty");
    return value;
}

const NlohmannJson &requiredObject(const NlohmannJson &object,
                                   const char *field,
                                   const std::string &path) {
    const auto *value = findField(object, field);
    if (!value || !value->is_object())
        throw std::invalid_argument(fieldPath(path, field) + " must be an object");
    return *value;
}

Transform parseTransform(const NlohmannJson &component, const std::string &path) {
    const auto anchorValue = integerOr(component, "anchor", Transform::Anchor::CC, path);
    if (anchorValue < Transform::Anchor::LT || anchorValue > Transform::Anchor::RD)
        throw std::invalid_argument(fieldPath(path, "anchor") + " must be between 0 and 8");

    return {
        .x = numberOr(component, "x", 0, path),
        .y = numberOr(component, "y", 0, path),
        .rotate = numberOr(component, "r", 0, path),
        .anchor = static_cast<Transform::Anchor>(anchorValue),
        .scaleX = numberOr(component, "scaleX", 1, path),
        .scaleY = numberOr(component, "scaleY", 1, path),
    };
}

HorizontalAlign parseHorizontalAlign(const NlohmannJson &component,
                                     const std::string &path,
                                     HorizontalAlign fallback) {
    const auto value = integerOr(component, "horizontalAlign", static_cast<int>(fallback), path);
    if (value < static_cast<int>(HorizontalAlign::Left) ||
        value > static_cast<int>(HorizontalAlign::Right))
        throw std::invalid_argument(fieldPath(path, "horizontalAlign") + " is invalid");
    return static_cast<HorizontalAlign>(value);
}

VerticalAlign parseVerticalAlign(const NlohmannJson &component,
                                 const std::string &path,
                                 VerticalAlign fallback) {
    const auto value = integerOr(component, "verticalAlign", static_cast<int>(fallback), path);
    if (value < static_cast<int>(VerticalAlign::BaseLine) ||
        value > static_cast<int>(VerticalAlign::Bottom))
        throw std::invalid_argument(fieldPath(path, "verticalAlign") + " is invalid");
    return static_cast<VerticalAlign>(value);
}

Unit parseUnit(const NlohmannJson &document) {
    const auto unit = lowercase(stringOr(document, "unit", "px", "template"));
    if (unit == "px" || unit == "pt")
        return Unit::Pixel;
    if (unit == "in" || unit == "inch")
        return Unit::Inch;
    if (unit == "mm")
        return Unit::Millimeter;
    if (unit == "cm")
        return Unit::Centimeter;
    throw std::invalid_argument("template.unit is unsupported: " + unit);
}

class Parser {
public:
    explicit Parser(ResourceLoader loader) : loader(std::move(loader)) {
    }

    ComponentPtr parseComponent(const NlohmannJson &component,
                                const std::string &path) {
        if (!component.is_object())
            throw std::invalid_argument(path + " must be an object");

        const auto type = lowercase(requiredString(component, "type", path));
        const auto name = stringOr(component, "name", {}, path);
        const auto transform = parseTransform(component, path);
        const auto width = numberOr(component, "w", 0, path);
        const auto height = numberOr(component, "h", 0, path);

        if (type == "rectangle") {
            const auto color = stringOr(component, "color", "#000000", path);
            return std::make_shared<Component::Rectangle>(
                name, transform, width, height, color.c_str());
        }

        if (type == "rectangleround") {
            const auto color = stringOr(component, "color", "#000000", path);
            const auto commonRadius = numberOr(component, "angle", 10, path);
            const auto topLeft = numberOr(component, "angleTL", commonRadius, path);
            const auto topRight = numberOr(component, "angleTR", commonRadius, path);
            const auto bottomRight = numberOr(component, "angleBR", commonRadius, path);
            const auto bottomLeft = numberOr(component, "angleBL", commonRadius, path);
            return std::make_shared<Component::RectangleRound>(
                name, transform, width, height,
                topLeft, topRight, bottomRight, bottomLeft, color.c_str());
        }

        if (type == "circle") {
            const auto color = stringOr(component, "color", "#000000", path);
            return std::make_shared<Component::Circle>(
                name, transform, width, height, color.c_str());
        }

        if (type == "image") {
            const auto source = requiredString(component, "src", path);
            return std::make_shared<Component::Image>(
                name, transform, resource(source, path), width, height);
        }

        if (type == "svg") {
#ifdef RSVG_FOUND
            const auto source = requiredString(component, "src", path);
            auto &data = resource(source, path);
            return std::make_shared<Component::Svg>(
                name, transform, width, height, data.data(), data.size());
#else
            throw std::invalid_argument(path + " uses svg, but SVG support is disabled");
#endif
        }

        if (type == "mask") {
            auto mask = parseComponent(requiredObject(component, "mask", path),
                                       fieldPath(path, "mask"));
            auto child = parseComponent(requiredObject(component, "child", path),
                                        fieldPath(path, "child"));
            return std::make_shared<Component::Mask>(
                name, width, height, transform, std::move(mask), std::move(child));
        }

        if (type == "text") {
#ifdef PANGO_FOUND
            const auto content = stringOr(component, "content", {}, path);
            const auto color = stringOr(component, "color", "#000000", path);
            const auto fontFamily = stringOr(component, "fontFamily", {}, path);
            const auto fontWeight = integerOr(component, "fontWeight", 400, path);
            const auto fontSize = numberOr(component, "fontSize", 14, path);
            const auto maxWidth = numberOr(component, "maxWidth", 0, path);
            const auto lineSpacing = numberOr(component, "lineSpacing", 0, path);
            const auto wordSpacing = numberOr(component, "wordSpacing", 0, path);
            return std::make_shared<Component::Text>(
                name, transform, content, color.c_str(), fontFamily,
                fontWeight, fontSize,
                parseHorizontalAlign(component, path, HorizontalAlign::Left),
                parseVerticalAlign(component, path, VerticalAlign::BaseLine),
                maxWidth, lineSpacing, wordSpacing);
#else
            throw std::invalid_argument(path + " uses text, but text support is disabled");
#endif
        }

        if (type == "textarea") {
#ifdef PANGO_FOUND
            const auto content = stringOr(component, "content", {}, path);
            const auto color = stringOr(component, "color", "#000000", path);
            const auto fontFamily = stringOr(component, "fontFamily", {}, path);
            const auto fontWeight = integerOr(component, "fontWeight", 400, path);
            const auto fontSize = numberOr(component, "fontSize", 14, path);
            const auto lineSpacing = numberOr(component, "lineSpacing", 0, path);
            const auto wordSpacing = numberOr(component, "wordSpacing", 0, path);
            return std::make_shared<Component::TextArea>(
                name, transform, width, height, content, color.c_str(), fontFamily,
                fontWeight, fontSize,
                parseHorizontalAlign(component, path, HorizontalAlign::Left),
                parseVerticalAlign(component, path, VerticalAlign::Top),
                lineSpacing, wordSpacing);
#else
            throw std::invalid_argument(path + " uses textArea, but text support is disabled");
#endif
        }

        if (type == "group") {
            const auto *children = findField(component, "child");
            if (!children || !children->is_array())
                throw std::invalid_argument(fieldPath(path, "child") + " must be an array");
            auto group = std::make_shared<Component::Group>(name, transform);
            for (size_t index = 0; index < children->size(); ++index) {
                group->addChild(parseComponent(
                    (*children)[index],
                    fieldPath(path, "child") + "[" + std::to_string(index) + "]"));
            }
            return group;
        }

        if (type == "repeat") {
            auto repeat = std::make_shared<Component::Repeat>(name, transform, width, height);
            const auto *children = findField(component, "child");
            if (!children)
                return repeat;
            if (children->is_array()) {
                for (size_t index = 0; index < children->size(); ++index) {
                    repeat->addChild(parseComponent(
                        (*children)[index],
                        fieldPath(path, "child") + "[" + std::to_string(index) + "]"));
                }
            } else if (children->is_object()) {
                repeat->addChild(parseComponent(*children, fieldPath(path, "child")));
            } else {
                throw std::invalid_argument(
                    fieldPath(path, "child") + " must be an object or array");
            }
            return repeat;
        }

        throw std::invalid_argument(fieldPath(path, "type") + " is unsupported: " + type);
    }

private:
    const std::vector<uint8_t> &resource(const std::string &source,
                                         const std::string &path) {
        const auto existing = resources.find(source);
        if (existing != resources.end())
            return existing->second;
        if (!loader)
            throw std::invalid_argument(
                fieldPath(path, "src") + " requires a resource loader: " + source);
        auto data = loader(source);
        if (data.empty())
            throw std::invalid_argument(fieldPath(path, "src") + " is empty: " + source);
        return resources.emplace(source, std::move(data)).first->second;
    }

    ResourceLoader loader;
    std::unordered_map<std::string, std::vector<uint8_t>> resources;
};

Document parseDocument(const NlohmannJson &json, ResourceLoader resourceLoader) {
    if (!json.is_object())
        throw std::invalid_argument("Template JSON root must be an object");

    Document document;
    document.width = numberOr(json, "w", document.width, "template");
    document.height = numberOr(json, "h", document.height, "template");
    if (document.width <= 0 || document.height <= 0)
        throw std::invalid_argument("Template canvas dimensions must be positive finite numbers");
    document.unit = parseUnit(json);
    document.ppi = numberOr(json, "ppi", document.ppi, "template");
    if (document.ppi <= 0)
        throw std::invalid_argument("template.ppi must be a positive finite number");

    Parser parser(std::move(resourceLoader));
    document.body = parser.parseComponent(
        requiredObject(json, "body", "template"), "template.body");
    return document;
}

} // namespace

Document parseTemplate(const std::vector<uint8_t> &json,
                       ResourceLoader resourceLoader) {
    if (json.empty())
        throw std::invalid_argument("Template JSON is empty");
    return parseDocument(NlohmannJson::parse(json.begin(), json.end()),
                         std::move(resourceLoader));
}

Document parseTemplate(const std::string &json,
                       ResourceLoader resourceLoader) {
    if (json.empty())
        throw std::invalid_argument("Template JSON is empty");
    return parseDocument(NlohmannJson::parse(json), std::move(resourceLoader));
}

} // namespace ArtRobot::Json
