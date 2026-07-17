# ArtRobot

ArtRobot 是一个面向简易绘图与排版场景的 Rust 库。它以 Cairo、Pango、`image` 和 `resvg` 为基础，提供统一的组件、变换与渲染接口。

## 功能

- 基础组件：矩形、圆角矩形、椭圆、文字、文本区、图片和 SVG
- 组合组件：`Group`、`Mask` 和区域平铺 `Repeat`
- 变换：位置、旋转、缩放和九宫格锚点
- 图片输入：PNG、JPEG、WebP，以及原始 BGRA 像素
- 输出格式：SVG、PDF、Cairo ARGB32 像素、PNG、JPEG 和 WebP
- 图片缩放：只传宽或高时自动保持原始宽高比
- 错误处理：无效图片、SVG 和原始像素输入通过 `art_robot::Result` 返回错误

导入的 SVG 由 `resvg` 栅格化后加入 Cairo 画布；Rust 原生图形和文字在 SVG/PDF 输出中仍使用 Cairo 的矢量绘制命令。

## 系统要求

- Rust 1.88 或更高版本
- Cairo 1.16 或更高版本
- Pango / PangoCairo

macOS 使用 Homebrew 时可安装系统依赖：

```sh
brew install cairo pango
```

## 示例

```rust
use art_robot::component::{Component, Rectangle};
use art_robot::renderer::{ImageType, RendererImage, Unit};
use art_robot::types::{Anchor, Color, Transform};

fn main() -> art_robot::Result<()> {
    let background = Rectangle::new(
        "background".into(),
        Transform {
            anchor: Anchor::LT,
            ..Transform::default()
        },
        320.0,
        200.0,
        Color::AQUA,
    );

    let renderer = RendererImage::new(320.0, 200.0, Unit::Pixel, 72.0);
    renderer.render(background.surface())?;
    renderer.save("output.png", ImageType::Png)?;
    Ok(())
}
```

图片和 SVG 可从文件或内存创建：

```rust
use art_robot::component::{Image, Svg};
use art_robot::types::Transform;

# fn load() -> art_robot::Result<()> {
let image = Image::from_file(
    "photo".into(),
    Transform::default(),
    "photo.webp",
    Some(640.0),
    None,
)?;

let svg = Svg::from_file(
    "logo".into(),
    Transform::default(),
    "logo.svg",
    Some(200.0),
    Some(100.0),
)?;
# let _ = (image, svg);
# Ok(())
# }
```

## 验证

```sh
cargo test --all-targets
cargo clippy --all-targets -- -D warnings
```

回归测试覆盖图片编解码往返、原始 BGRA、SVG、Mask、Repeat、颜色解析、变换、矢量输出与单位换算。
