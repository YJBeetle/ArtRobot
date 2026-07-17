# ArtRobot

ArtRobot 是一个面向简易绘图与排版场景的 C++17 静态库，在 Cairo、Pango、librsvg、libjpeg 和 libwebp 之上提供统一的组件与渲染接口。

## 功能

- 基础组件：矩形、圆角矩形、椭圆、文字、文本区、图片和 SVG
- 组合组件：Group、Mask 和 Repeat 平铺
- 变换：位置、旋转、缩放和九宫格锚点
- 输出格式：SVG、PDF、原始像素、PNG、JPEG 和 WebP
- 图片输入：PNG、JPEG、WebP、BGRA 原始数据，可选 OpenCV `cv::Mat`
- 可选 `ArtRobotJson`：将 JSON 模板解析为同一套组件树

组件的 `Transform` 在构造时应用。图片或 SVG 数据无法载入时，构造函数会抛出 `std::invalid_argument` 或 `std::runtime_error`。

## 依赖项

Pixman 和 Cairo 是必需依赖。以下功能默认启用，可以通过对应的 CMake 选项关闭：

| 功能 | 依赖 | CMake 选项 |
| --- | --- | --- |
| 文字 | Pango | `UsePango` |
| SVG | librsvg | `UseRsvg` |
| JPEG | libjpeg 或 jpeg-turbo | `UseJpeg` |
| WebP | libwebp | `UseWebP` |
| `cv::Mat` | OpenCV | `UseOpenCV` |
| JSON 模板 | nlohmann/json 子模块 | `UseJsonTemplate` |

## 编译

```sh
cmake -S . -B build
cmake --build build -j8
```

未安装 OpenCV 时可以关闭对应功能：

```sh
cmake -S . -B build -DUseOpenCV=OFF
```

启用 JSON 模板模块前需要递归初始化子模块：

```sh
git submodule update --init --recursive
cmake -S . -B build -DUseJsonTemplate=ON
```

启用后会额外提供 `ArtRobotJson` target 和 `ArtRobot::Json` alias；核心
`ArtRobot` target 不会强制链接 JSON 依赖。

## JSON 模板

`ArtRobotJson` 负责画布参数、组件树、默认值和字段校验。资源由调用方通过
`ResourceLoader` 提供，因此同一解析器既能用于本地文件，也能用于 WASM
内存资源：

```cpp
#include <ArtRobot/Json/Template.hpp>

auto document = ArtRobot::Json::parseTemplate(
    jsonBytes,
    [](const std::string &source) {
        return readFile(source);
    });

ArtRobot::Renderer renderer(
    ArtRobot::OutputType::Png,
    document.width,
    document.height,
    document.unit,
    document.ppi);
renderer.render(document.body->getSurface());
```

当前解析组件包括 `rectangle`、`rectangleRound`、`circle`、`image`、
`mask`、`group`、`repeat`，以及功能启用时的 `text`、`textArea` 和 `svg`。
未知或不可用组件会抛出带字段路径的异常。资源在一次模板解析中按 `src`
缓存，同一素材只调用一次加载器。

## 测试

测试默认随项目构建，通过 `BuildTest=OFF` 可以关闭：

```sh
ctest --test-dir build --output-on-failure
```

当前回归覆盖基础组件、分组、蒙版、Repeat、变换、PNG/JPEG/WebP 图片输入、SVG 解析，以及 PNG、JPEG、PDF、WebP 输出。

## 示例

```cpp
#include <ArtRobot/ArtRobot.hpp>

int main() {
    using namespace ArtRobot;

    Component::Rectangle rectangle(
        "background",
        {.anchor = Transform::LT},
        320,
        200,
        Color::Aqua);

    Renderer renderer(OutputType::Png, 320, 200);
    renderer.render(rectangle.getSurface());
    renderer.saveToFile("output.png");
}
```
