# ArtRobot

ArtRobot 是一个面向简易绘图与排版场景的 C++17 静态库，在 Cairo、Pango、librsvg、libjpeg 和 libwebp 之上提供统一的组件与渲染接口。

## 功能

- 基础组件：矩形、圆角矩形、椭圆、文字、文本区、图片和 SVG
- 组合组件：Group、Mask 和 Repeat 平铺
- 变换：位置、旋转、缩放和九宫格锚点
- 输出格式：SVG、PDF、原始像素、PNG、JPEG 和 WebP
- 图片输入：PNG、JPEG、WebP、DDS、BGRA 原始数据，可选 OpenCV `cv::Mat`

DDS 输入用于跳过运行时图片解码。当前接受 DDS DX10 的严格子集：2D、单层、
无 mipmap、`DXGI_FORMAT_B8G8R8A8_UNORM`，并且 alpha mode 必须是
premultiplied 或 opaque。像素数据会按 DDS pitch 复制到 Cairo 自有的
`CAIRO_FORMAT_ARGB32` surface，因此不会引用调用者的临时缓冲区。
- 可选 `ArtRobotJson`：将 JSON 模板解析为同一套组件树
- 可选 `ArtRobotRender`：直接从命令行渲染 JSON 模板

组件的 `Transform` 在构造时应用。图片或 SVG 数据无法载入时，构造函数会抛出 `std::invalid_argument` 或 `std::runtime_error`。

## 依赖项

Pixman 和 Cairo 是必需依赖。以下功能通过对应 CMake 选项控制；JSON 模板
默认关闭，其余功能默认启用：

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

构建随仓库提供的命令行工具：

```sh
cmake -S . -B build \
  -DBuildArtRobotRender=ON \
  -DUseOpenCV=OFF
cmake --build build --parallel 8
```

可执行文件和示例模板位于 `build/tools/ArtRobotRender/`。原
ArtRobotRender 仓库的历史已经通过双 parent merge commit 保留在本仓库，
工具源码位于 [`tools/ArtRobotRender/`](tools/ArtRobotRender/)。

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

当前回归覆盖基础组件、分组、蒙版、Repeat、变换、JSON 模板解析、资源
加载与缓存、PNG/JPEG/WebP 图片输入、SVG 解析，以及 PNG、JPEG、PDF、
WebP 输出。

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
