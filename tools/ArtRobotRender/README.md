# ArtRobotRender

ArtRobotRender 是基于 [ArtRobot](https://github.com/YJBeetle/ArtRobot) 和
可选 `ArtRobotJson` 模块的 JSON 图形渲染命令行工具。它读取组件树描述，
组合图形、文字、图片和 SVG，并输出 SVG、PDF、PNG、JPEG 或 WebP 文件。

## 功能

- 基础组件：矩形、圆角矩形、椭圆、文字、文本区、图片和 SVG
- 组合组件：Group、Mask 和 Repeat 平铺
- 变换：位置、旋转、缩放和九宫格锚点
- 画布单位：像素、英寸、毫米和厘米
- 输出格式：SVG、PDF、PNG、JPEG 和 WebP
- 内置 demo、ThrowIt 和 SaySticker 示例模板

## 获取源码

ArtRobotRender 已作为可选 executable target 合并进 ArtRobot。JSON 依赖以
嵌套 submodule 固定版本，因此克隆时需要递归初始化：

```sh
git clone --recurse-submodules https://github.com/YJBeetle/ArtRobot.git
cd ArtRobot
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

`nlohmann/json` 由 `3rdParty/json` submodule 固定版本。ArtRobot 支持可选
OpenCV 输入，但 ArtRobotRender 本身不使用 OpenCV，因此下面的构建命令
将其关闭。

## 构建

```sh
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DBuildArtRobotRender=ON \
  -DUseOpenCV=OFF
cmake --build build --parallel 8
```

## 使用

可执行文件和随构建复制的模板位于 `build/tools/ArtRobotRender/`。JSON 中的
图片和 SVG 相对路径以 JSON 文件所在目录为基准解析，因此可以从任意工作
目录执行：

```sh
cd build/tools/ArtRobotRender
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
| `body` | 根组件对象 | 必填 |

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
| `repeat` | 区域宽高及 `child` 对象或数组 |
| `group` | `child` 组件数组 |

完整示例参见 [`template/`](template/) 目录。

组件字段由 `ArtRobotJson` 严格校验。未知组件、错误字段类型、无效画布尺寸
或无法加载的资源会输出包含组件路径的错误并以非零状态退出。

### StickerGenerator 输入扩展

模板可以通过顶层 `inputs` 声明允许调用方覆盖的参数，并在组件的
`bindings` 中将组件属性绑定到输入名称：

```json
{
  "inputs": {
    "avatar": {
      "type": "image",
      "description": "头像"
    }
  },
  "body": {
    "type": "image",
    "src": "default-avatar.jpg",
    "bindings": {
      "src": "avatar"
    }
  }
}
```

ArtRobotRender 命令行工具目前忽略 `inputs` 和 `bindings`，使用组件原始
字段渲染默认结果。Workers-StickerGenerator 会在渲染前验证输入并应用覆盖。

## 测试

ArtRobot 的测试默认随主项目一起构建：

```sh
ctest --test-dir build --output-on-failure --no-tests=error
```

测试覆盖组件组合、蒙版、Repeat、变换、JSON 模板解析、资源加载与缓存、
PNG/JPEG/WebP 图片输入、SVG 解析，以及 PNG、JPEG、PDF 和 WebP 输出。
若只需要构建命令行程序，可以在配置时添加 `-DBuildTest=OFF`。

## CI

ArtRobot 的 GitHub Actions 会在 Ubuntu 24.04 上递归检出 submodule，安装
依赖，执行 Release 构建和全部测试，并实际渲染 ThrowIt 模板验证 PNG 输出。
测试日志和示例图片会作为 CI artifact 保留 7 天。

## License

本项目使用 [GNU General Public License v3.0](../../LICENSE)。
