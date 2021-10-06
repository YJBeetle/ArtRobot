# ArtRobot

一个用于简易绘图与排版需求的对一堆开源库的C++封装，可输出矢量与位图。

文档什么的完善中……

## 依赖

* cairo
* pango
* librsvg
* opencv
* webp

## 编译
    cmake -B build && cmake --build build -j28

## 测试

    cd build
    make -j28 && ./OctopusRender ThrowIt.json -t png out.png
    open out.png

## OctopusRender 格式

格式请参考 [Octopus Documents](https://gitlab.com/project-null-plus-1/octopus-docs) 中的 [数据交换JSON格式说明文档](https://gitlab.com/project-null-plus-1/octopus-docs/blob/master/DataExchangeJsonFormat.md)
