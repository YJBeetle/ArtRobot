Art_robot
=========

依赖
---

* cairo
* freetype
* librsvg

可用的命令行结构
--------------

`Art_robot data.json`
`Art_robot -j data.json -o a.pdf`

可用的json数据结构
--------------

```
{
  "type":"SVG",
  "width":420,
  "height":297,
  "unit":"px",
  "ppi":72,
  "draw":[
    {
    "type":"rectangle",
      "color":"FCF7E8",
      "x":0,
      "y":0,
      "width":420,
      "height":297
    },
    {
      "type":"svgfile",
      "filename":"img\/bg-veins.svg",
      "x":0,
      "y":0,
      "width":420,
      "height":297
    },
    {
      "type":"rectangle",
      "color":"686767",
      "x":0,
      "y":0,
      "width":420,
      "height":16
    },
    {
      "type":"rectangle",
      "color":"686767",
      "x":0,
      "y":281,
      "width":420,
      "height":16
    },
    {
      "type":"text",
      "text":"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an",
      "font":"fonts\/Lantinghei.ttc",
      "face":2,
      "size":5,
      "alignment":1,
      "color":"ffffff",
      "x":210,
      "y":10
    },
    {
      "type":"text",
      "text":"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an",
      "font":"fonts\/Lantinghei.ttc",
      "face":2,
      "size":5,
      "alignment":1,
      "color":"ffffff",
      "x":210,
      "y":291
    },
    {
      "type":"svgfile",
      "filename":"img\/logo.svg",
      "x":185,
      "y":30,
      "width":50,
      "height":37.544
    },
    {
      "type":"text",
      "text":"会议名称",
      "font":"fonts\/Yuanti.ttc",
      "face":2,
      "size":26,
      "alignment":1,
      "color":"686767",
      "x":210,
      "y":100
    },
    {
      "type":"text",
      "text":"会议名称行2",
      "font":"fonts\/Yuanti.ttc",
      "face":2,
      "size":26,
      "alignment":1,
      "color":"686767",
      "x":210,
      "y":130
    },
    {
      "type":"text",
      "text":"会议时间",
      "font":"fonts\/Yuanti.ttc",
      "face":2,
      "size":18,
      "alignment":1,
      "color":"686767",
      "x":210,
      "y":170
    },
    {
      "type":"text",
      "text":"会议地点",
      "font":"fonts\/Yuanti.ttc",
      "face":2,
      "size":18,
      "alignment":1,
      "color":"686767",
      "x":210,
      "y":190
    },
    {
      "type":"text",
      "text":"Date",
      "font":"fonts\/Yuanti.ttc",
      "face":1,
      "size":18,
      "alignment":1,
      "color":"686767",
      "x":210,
      "y":220
    },
    {
      "type":"text",
      "text":"Location",
      "font":"fonts\/Yuanti.ttc",
      "face":1,
      "size":18,
      "alignment":1,
      "color":"686767",
      "x":210,
      "y":240
    }
  ]
}
```


```
{
  "type":"PDF",
  "width":420,
  "height":297,
  "unit":"mm",
  "ppi":72,
  "draw":[
    [
      {
      "type":"rectangle",
        "color":"FCF7E8",
        "x":0,
        "y":0,
        "width":420,
        "height":297
      },
      {
        "type":"svgfile",
        "filename":"img\/bg-veins.svg",
        "x":0,
        "y":0,
        "width":420,
        "height":297
      },
      {
        "type":"rectangle",
        "color":"686767",
        "x":0,
        "y":0,
        "width":420,
        "height":16
      },
      {
        "type":"rectangle",
        "color":"686767",
        "x":0,
        "y":281,
        "width":420,
        "height":16
      },
      {
        "type":"text",
        "text":"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an",
        "font":"fonts\/Lantinghei.ttc",
        "face":2,
        "size":5,
        "alignment":1,
        "color":"ffffff",
        "x":210,
        "y":10
      },
      {
        "type":"text",
        "text":"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an",
        "font":"fonts\/Lantinghei.ttc",
        "face":2,
        "size":5,
        "alignment":1,
        "color":"ffffff",
        "x":210,
        "y":291
      },
      {
        "type":"svgfile",
        "filename":"img\/logo.svg",
        "x":185,
        "y":30,
        "width":50,
        "height":37.544
      },
      {
        "type":"text",
        "text":"会议名称",
        "font":"fonts\/Yuanti.ttc",
        "face":2,
        "size":26,
        "alignment":1,
        "color":"686767",
        "x":210,
        "y":100
      },
      {
        "type":"text",
        "text":"会议名称行2",
        "font":"fonts\/Yuanti.ttc",
        "face":2,
        "size":26,
        "alignment":1,
        "color":"686767",
        "x":210,
        "y":130
      },
      {
        "type":"text",
        "text":"会议时间",
        "font":"fonts\/Yuanti.ttc",
        "face":2,
        "size":18,
        "alignment":1,
        "color":"686767",
        "x":210,
        "y":170
      },
      {
        "type":"text",
        "text":"会议地点",
        "font":"fonts\/Yuanti.ttc",
        "face":2,
        "size":18,
        "alignment":1,
        "color":"686767",
        "x":210,
        "y":190
      },
      {
        "type":"text",
        "text":"Date",
        "font":"fonts\/Yuanti.ttc",
        "face":1,
        "size":18,
        "alignment":1,
        "color":"686767",
        "x":210,
        "y":220
      },
      {
        "type":"text",
        "text":"Location",
        "font":"fonts\/Yuanti.ttc",
        "face":1,
        "size":18,
        "alignment":1,
        "color":"686767",
        "x":210,
        "y":240
      }
    ],
    [
      {
        "type":"pngfile",
        "filename":"img\/1.png",
        "x":350,
        "y":250,
        "width":50,
        "height":50
      }
    ]
  ]
}
```
