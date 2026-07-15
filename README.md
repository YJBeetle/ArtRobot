# ArtRobot

ArtRobot 是一个面向简易绘图与排版场景的 C++17 静态库，在 Cairo、Pango、librsvg、libjpeg 和 libwebp 之上提供统一的组件与渲染接口。

## 功能

- 基础组件：矩形、圆角矩形、椭圆、文字、文本区、图片和 SVG
- 组合组件：Group、Mask 和 Repeat 平铺
- 变换：位置、旋转、缩放和九宫格锚点
- 输出格式：SVG、PDF、原始像素、PNG、JPEG 和 WebP
- 图片输入：PNG、JPEG、WebP、BGRA 原始数据，可选 OpenCV `cv::Mat`

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

## 编译

```sh
cmake -S . -B build
cmake --build build -j8
```

未安装 OpenCV 时可以关闭对应功能：

```sh
cmake -S . -B build -DUseOpenCV=OFF
```

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
