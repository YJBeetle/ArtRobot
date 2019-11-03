# ArtRobot

## 依赖

* cairo
* freetype
* librsvg
* opencv

## 格式

格式请参考 [Octopus Documents](https://gitlab.com/project-null-plus-1/octopus-docs) 中的 [数据交换JSON格式说明文档](https://gitlab.com/project-null-plus-1/octopus-docs/blob/master/DataExchangeJsonFormat.md)

## 编译&测试

    mkdir build && cd build && cmake ..
    make -j28 && ./OctopusRender ThrowIt.json -t png out.png
    open out.png
    