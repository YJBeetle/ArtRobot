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
#include <unordered_set>
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

double literalNumberOr(const NlohmannJson &object,
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

struct Measurement {
    double realWidth = 0;
    double realHeight = 0;
};

using Measurements = std::unordered_map<std::string, Measurement>;

class NumericResolver {
public:
    NumericResolver(const NlohmannJson &layout, Measurements measurements)
            : measurements(std::move(measurements)) {
        const auto *declaredVariables = findField(layout, "variables");
        if (!declaredVariables)
            return;
        if (!declaredVariables->is_object())
            throw std::invalid_argument("template.layout.variables must be an object");
        variables = *declaredVariables;
    }

    double resolve(const NlohmannJson &expression, const std::string &path) {
        if (expression.is_number()) {
            const auto number = expression.get<double>();
            if (!std::isfinite(number))
                throw std::invalid_argument(path + " must be finite");
            return number;
        }
        if (!expression.is_object())
            throw std::invalid_argument(path + " must be a number or numeric expression");

        if (const auto *variable = findField(expression, "var")) {
            if (expression.size() != 1 || !variable->is_string() ||
                variable->get_ref<const std::string &>().empty())
                throw std::invalid_argument(path + ".var must be the expression's only non-empty string field");
            return resolveVariable(variable->get_ref<const std::string &>(), path);
        }

        if (const auto *measure = findField(expression, "measure")) {
            if (expression.size() != 1 || !measure->is_string())
                throw std::invalid_argument(path + ".measure must be the expression's only string field");
            return resolveMeasurement(measure->get_ref<const std::string &>(), path);
        }

        const auto *operation = findField(expression, "op");
        const auto *arguments = findField(expression, "args");
        if (!operation || !arguments || expression.size() != 2 ||
            !operation->is_string() || !arguments->is_array())
            throw std::invalid_argument(
                path + " must contain either var, measure, or op with args");

        const auto op = lowercase(operation->get<std::string>());
        std::vector<double> values;
        values.reserve(arguments->size());
        for (size_t index = 0; index < arguments->size(); ++index) {
            values.push_back(resolve(
                (*arguments)[index],
                path + ".args[" + std::to_string(index) + "]"));
        }

        double result = 0;
        if (op == "add") {
            requireArgumentCount(op, values, 1, path, false);
            for (const auto value : values)
                result += value;
        } else if (op == "sub") {
            requireArgumentCount(op, values, 2, path);
            result = values[0] - values[1];
        } else if (op == "mul") {
            requireArgumentCount(op, values, 1, path, false);
            result = 1;
            for (const auto value : values)
                result *= value;
        } else if (op == "div") {
            requireArgumentCount(op, values, 2, path);
            if (values[1] == 0)
                throw std::invalid_argument(path + " divides by zero");
            result = values[0] / values[1];
        } else if (op == "min") {
            requireArgumentCount(op, values, 1, path, false);
            result = *std::min_element(values.begin(), values.end());
        } else if (op == "max") {
            requireArgumentCount(op, values, 1, path, false);
            result = *std::max_element(values.begin(), values.end());
        } else if (op == "clamp") {
            requireArgumentCount(op, values, 3, path);
            if (values[1] > values[2])
                throw std::invalid_argument(path + " clamp minimum exceeds maximum");
            result = std::clamp(values[0], values[1], values[2]);
        } else {
            throw std::invalid_argument(path + ".op is unsupported: " + op);
        }

        if (!std::isfinite(result))
            throw std::invalid_argument(path + " produced a non-finite number");
        return result;
    }

private:
    static void requireArgumentCount(const std::string &operation,
                                     const std::vector<double> &values,
                                     size_t expected,
                                     const std::string &path,
                                     bool exact = true) {
        const bool valid = exact ? values.size() == expected : values.size() >= expected;
        if (!valid) {
            throw std::invalid_argument(
                path + ".args must contain " +
                (exact ? std::to_string(expected) : "at least " + std::to_string(expected)) +
                " value(s) for " + operation);
        }
    }

    double resolveVariable(const std::string &name, const std::string &path) {
        const auto cached = resolvedVariables.find(name);
        if (cached != resolvedVariables.end())
            return cached->second;
        const auto declared = variables.find(name);
        if (declared == variables.end())
            throw std::invalid_argument(path + " references unknown layout variable: " + name);
        if (!resolvingVariables.insert(name).second)
            throw std::invalid_argument(path + " contains a layout variable cycle at: " + name);
        try {
            const auto value = resolve(*declared, "template.layout.variables." + name);
            resolvingVariables.erase(name);
            resolvedVariables.emplace(name, value);
            return value;
        } catch (...) {
            resolvingVariables.erase(name);
            throw;
        }
    }

    double resolveMeasurement(const std::string &reference,
                              const std::string &path) const {
        const auto separator = reference.rfind('.');
        if (separator == std::string::npos || separator == 0 ||
            separator + 1 >= reference.size())
            throw std::invalid_argument(
                path + ".measure must use '<component>.realW' or '<component>.realH'");
        const auto name = reference.substr(0, separator);
        const auto field = lowercase(reference.substr(separator + 1));
        const auto measurement = measurements.find(name);
        if (measurement == measurements.end())
            throw std::invalid_argument(
                path + " references an unknown measured text component: " + name);
        if (field == "realw")
            return measurement->second.realWidth;
        if (field == "realh")
            return measurement->second.realHeight;
        throw std::invalid_argument(
            path + ".measure must end in realW or realH: " + reference);
    }

    NlohmannJson variables = NlohmannJson::object();
    Measurements measurements;
    std::unordered_map<std::string, double> resolvedVariables;
    std::unordered_set<std::string> resolvingVariables;
};

double numberOr(const NlohmannJson &object,
                const char *field,
                double fallback,
                const std::string &path,
                NumericResolver *resolver = nullptr) {
    const auto *value = findField(object, field);
    if (!value || value->is_null())
        return fallback;
    if (resolver)
        return resolver->resolve(*value, fieldPath(path, field));
    return literalNumberOr(object, field, fallback, path);
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

Transform parseTransform(const NlohmannJson &component,
                         const std::string &path,
                         NumericResolver *resolver = nullptr) {
    const auto anchorValue = integerOr(component, "anchor", Transform::Anchor::CC, path);
    if (anchorValue < Transform::Anchor::LT || anchorValue > Transform::Anchor::RD)
        throw std::invalid_argument(fieldPath(path, "anchor") + " must be between 0 and 8");

    return {
        .x = numberOr(component, "x", 0, path, resolver),
        .y = numberOr(component, "y", 0, path, resolver),
        .rotate = numberOr(component, "r", 0, path, resolver),
        .anchor = static_cast<Transform::Anchor>(anchorValue),
        .scaleX = numberOr(component, "scaleX", 1, path, resolver),
        .scaleY = numberOr(component, "scaleY", 1, path, resolver),
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

void collectMeasurements(const NlohmannJson &component,
                         const std::string &path,
                         Measurements &measurements) {
    if (!component.is_object())
        throw std::invalid_argument(path + " must be an object");

    const auto type = lowercase(requiredString(component, "type", path));
    const auto name = stringOr(component, "name", {}, path);
#ifdef PANGO_FOUND
    if (!name.empty() && (type == "text" || type == "textarea")) {
        const auto content = stringOr(component, "content", {}, path);
        const auto color = stringOr(component, "color", "#000000", path);
        const auto fontFamily = stringOr(component, "fontFamily", {}, path);
        const auto fontWeight = integerOr(component, "fontWeight", 400, path);
        const auto fontSize = literalNumberOr(component, "fontSize", 14, path);
        const auto lineSpacing = literalNumberOr(component, "lineSpacing", 0, path);
        const auto wordSpacing = literalNumberOr(component, "wordSpacing", 0, path);

        Measurement measurement;
        if (type == "text") {
            Component::Text text(
                name, {}, content, color.c_str(), fontFamily,
                fontWeight, fontSize,
                parseHorizontalAlign(component, path, HorizontalAlign::Left),
                parseVerticalAlign(component, path, VerticalAlign::BaseLine),
                literalNumberOr(component, "maxWidth", 0, path),
                lineSpacing, wordSpacing);
            measurement = {text.realW(), text.realH()};
        } else {
            Component::TextArea text(
                name, {},
                literalNumberOr(component, "w", 0, path),
                literalNumberOr(component, "h", 0, path),
                content, color.c_str(), fontFamily,
                fontWeight, fontSize,
                parseHorizontalAlign(component, path, HorizontalAlign::Left),
                parseVerticalAlign(component, path, VerticalAlign::Top),
                lineSpacing, wordSpacing);
            measurement = {text.realW(), text.realH()};
        }
        if (!measurements.emplace(name, measurement).second)
            throw std::invalid_argument(
                path + ".name duplicates a measured text component: " + name);
    }
#else
    if (!name.empty() && (type == "text" || type == "textarea"))
        throw std::invalid_argument(path + " requires text support for layout measurement");
#endif

    const auto *children = findField(component, "child");
    if (children) {
        if (children->is_array()) {
            for (size_t index = 0; index < children->size(); ++index) {
                collectMeasurements(
                    (*children)[index],
                    fieldPath(path, "child") + "[" + std::to_string(index) + "]",
                    measurements);
            }
        } else if (children->is_object()) {
            collectMeasurements(*children, fieldPath(path, "child"), measurements);
        }
    }
    const auto *mask = findField(component, "mask");
    if (mask && mask->is_object())
        collectMeasurements(*mask, fieldPath(path, "mask"), measurements);
}

class Parser {
public:
    Parser(ResourceLoader loader, NumericResolver *resolver)
            : loader(std::move(loader)), resolver(resolver) {
    }

    ComponentPtr parseComponent(const NlohmannJson &component,
                                const std::string &path) {
        if (!component.is_object())
            throw std::invalid_argument(path + " must be an object");

        const auto type = lowercase(requiredString(component, "type", path));
        const auto name = stringOr(component, "name", {}, path);
        const auto transform = parseTransform(component, path, resolver);
        const auto width = numberOr(component, "w", 0, path, resolver);
        const auto height = numberOr(component, "h", 0, path, resolver);

        if (type == "rectangle") {
            const auto color = stringOr(component, "color", "#000000", path);
            return std::make_shared<Component::Rectangle>(
                name, transform, width, height, color.c_str());
        }

        if (type == "rectangleround") {
            const auto color = stringOr(component, "color", "#000000", path);
            const auto commonRadius = numberOr(component, "angle", 10, path, resolver);
            const auto topLeft = numberOr(component, "angleTL", commonRadius, path, resolver);
            const auto topRight = numberOr(component, "angleTR", commonRadius, path, resolver);
            const auto bottomRight = numberOr(component, "angleBR", commonRadius, path, resolver);
            const auto bottomLeft = numberOr(component, "angleBL", commonRadius, path, resolver);
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
            const auto fontSize = numberOr(component, "fontSize", 14, path, resolver);
            const auto maxWidth = numberOr(component, "maxWidth", 0, path, resolver);
            const auto lineSpacing = numberOr(component, "lineSpacing", 0, path, resolver);
            const auto wordSpacing = numberOr(component, "wordSpacing", 0, path, resolver);
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
            const auto fontSize = numberOr(component, "fontSize", 14, path, resolver);
            const auto lineSpacing = numberOr(component, "lineSpacing", 0, path, resolver);
            const auto wordSpacing = numberOr(component, "wordSpacing", 0, path, resolver);
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
    NumericResolver *resolver;
    std::unordered_map<std::string, std::vector<uint8_t>> resources;
};

Document parseDocument(const NlohmannJson &json, ResourceLoader resourceLoader) {
    if (!json.is_object())
        throw std::invalid_argument("Template JSON root must be an object");

    const auto &body = requiredObject(json, "body", "template");
    std::unique_ptr<NumericResolver> resolver;
    if (const auto *layout = findField(json, "layout")) {
        if (!layout->is_object())
            throw std::invalid_argument("template.layout must be an object");
        Measurements measurements;
        collectMeasurements(body, "template.body", measurements);
        resolver = std::make_unique<NumericResolver>(*layout, std::move(measurements));
    }

    Document document;
    document.width = numberOr(json, "w", document.width, "template", resolver.get());
    document.height = numberOr(json, "h", document.height, "template", resolver.get());
    if (document.width <= 0 || document.height <= 0)
        throw std::invalid_argument("Template canvas dimensions must be positive finite numbers");
    document.unit = parseUnit(json);
    document.ppi = numberOr(json, "ppi", document.ppi, "template", resolver.get());
    if (document.ppi <= 0)
        throw std::invalid_argument("template.ppi must be a positive finite number");

    Parser parser(std::move(resourceLoader), resolver.get());
    document.body = parser.parseComponent(body, "template.body");
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
