# ArtRobotRender

ArtRobotRender 是基于 [ArtRobot](https://github.com/YJBeetle/ArtRobot) 的 JSON 图形渲染命令行工具。它读取组件树描述，组合图形、文字、图片和 SVG，并输出 SVG、PDF、PNG、JPEG 或 WebP 文件。

## 功能

- 基础组件：矩形、圆角矩形、椭圆、文字、文本区、图片和 SVG
- 组合组件：Group、Mask 和 Repeat 平铺
- 变换：位置、旋转、缩放和九宫格锚点
- 画布单位：像素、英寸、毫米和厘米
- 输出格式：SVG、PDF、PNG、JPEG 和 WebP
- 内置 demo、ThrowIt 和 SaySticker 示例模板

## 获取源码

ArtRobot 以 Git submodule 的形式放在 `ArtRobot/`，克隆时需要一并初始化：

```sh
git clone --recurse-submodules https://github.com/YJBeetle/ArtRobotRender.git
cd ArtRobotRender
```

已有仓库可以执行：

```sh
git submodule update --init --recursive
```

## 依赖

项目使用 C++17 和 CMake，运行完整功能需要：

- Pixman
- Cairo
- Pango
- librsvg
- libjpeg 或 jpeg-turbo
- libwebp

Ubuntu 24.04：

```sh
sudo apt-get update
sudo apt-get install --yes --no-install-recommends \
  build-essential cmake ninja-build pkg-config \
  libpixman-1-dev libcairo2-dev libpango1.0-dev \
  librsvg2-dev libjpeg-dev libwebp-dev fonts-noto-cjk
```

macOS（Homebrew）：

```sh
brew install cmake pkg-config pixman cairo pango librsvg jpeg-turbo webp
```

`nlohmann/json` 由 CMake 在首次配置时自动下载。ArtRobot 支持可选 OpenCV 输入，但 ArtRobotRender 本身不使用 OpenCV，因此下面的构建命令将其关闭。

## 构建

```sh
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DUseOpenCV=OFF
cmake --build build --parallel 8
```

`CMAKE_POLICY_VERSION_MINIMUM` 用于兼容当前 JSON 依赖的旧 CMake 声明，在较旧的 CMake 版本上保留该参数也不会影响构建。

## 使用

推荐从 `build` 目录运行随项目复制的示例，因为 JSON 中的图片和 SVG 路径相对程序的当前工作目录解析：

```sh
cd build
./ArtRobotRender -t png demo.json demo-output.png
```

通用调用方式：

```text
ArtRobotRender [选项] <JSON 文件> [输出文件]

选项：
  -f, --jsonfile <路径>   输入 JSON 文件
  -o, --output <路径>     输出文件
  -t, --type <格式>       svg、pdf、png、jpg、jpeg 或 webp
  -V, --version           显示版本信息
      --help              显示帮助
```

也可以完全使用选项指定输入和输出：

```sh
./ArtRobotRender -f demo.json -o demo-output.webp -t webp
```

输出类型默认为 SVG，不会根据输出文件扩展名自动推断；生成 PNG、JPEG、PDF 或 WebP 时必须显式传入 `-t`。

## JSON 格式

顶层文档包含画布尺寸和一个根组件：

```json
{
  "w": 320,
  "h": 200,
  "unit": "px",
  "ppi": 72,
  "body": {
    "type": "rectangle",
    "name": "background",
    "x": 0,
    "y": 0,
    "w": 320,
    "h": 200,
    "anchor": 0,
    "color": "#20242A"
  }
}
```

画布字段：

| 字段 | 说明 | 默认值 |
| --- | --- | --- |
| `w`、`h` | 画布宽度和高度 | `200` |
| `unit` | `px`、`pt`、`in`、`inch`、`mm` 或 `cm` | `px` |
| `ppi` | 非像素单位使用的每英寸像素数 | `72` |
| `body` | 根组件对象 | 空组件 |

所有组件都可以使用 `name`、`x`、`y`、`w`、`h`、`r`、`anchor`、`scaleX` 和 `scaleY`。`r` 为旋转角度；`anchor` 取值从 `0` 到 `8`，依次对应左上、中上、右上、左中、中心、右中、左下、中下和右下。

支持的组件类型：

| `type` | 主要字段 |
| --- | --- |
| `rectangle` | `color` |
| `rectangleRound` | `color`、`angle`，或四角半径 `angleTL`、`angleTR`、`angleBR`、`angleBL` |
| `circle` | `color` |
| `svg` | `src` |
| `image` | `src` |
| `text` | `content`、`color`、`fontFamily`、`fontWeight`、`fontSize`、对齐和间距字段 |
| `textArea` | 文字字段及区域宽高 |
| `mask` | `mask` 和 `child` 两个子组件 |
| `repeat` | 区域宽高 |
| `group` | `child` 组件数组 |

完整示例参见 [`template/`](template/) 目录。

## 测试

ArtRobot 的测试默认随主项目一起构建：

```sh
ctest --test-dir build/ArtRobot --output-on-failure --no-tests=error
```

测试覆盖组件组合、蒙版、Repeat、变换、PNG/JPEG/WebP 图片输入、SVG 解析，以及 PNG、JPEG、PDF 和 WebP 输出。若只需要构建命令行程序，可以在配置时添加 `-DBuildTest=OFF`。

## CI

GitHub Actions 会在 Ubuntu 24.04 上递归检出 submodule，安装依赖，执行 Release 构建和全部测试，并实际渲染 `demo.json` 验证 PNG 输出。测试日志和示例图片会作为 CI artifact 保留 7 天。

## License

本项目使用 [GNU General Public License v3.0](ArtRobot/LICENSE)。
