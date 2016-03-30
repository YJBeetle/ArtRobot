#!/usr/bin/php
<?php
$data=(object)[
	"outfile"=>"out.pdf",
	"type"=>"PDF",
	"width"=>420,
	"height"=>297,
	"draw"=>[
		(object)[
			"type"=>"rectangle",
			"color"=>"FCF7E8",
			"x"=>0,
			"y"=>0,
			"width"=>420,
			"height"=>297
		],
		(object)[
			"type"=>"svgfile",
			"filename"=>"bg-veins.svg",
			"x"=>0,
			"y"=>0,
			"width"=>420,
			"height"=>297
		],
		(object)[
			"type"=>"pngfile",
			"filename"=>"1.png",
			"x"=>10,
			"y"=>10,
			"width"=>100,
			"height"=>100
		],
		(object)[
			"type"=>"rectangle",
			"color"=>"686767",
			"x"=>0,
			"y"=>0,
			"width"=>420,
			"height"=>16
		],
		(object)[
			"type"=>"rectangle",
			"color"=>"686767",
			"x"=>0,
			"y"=>297-16,
			"width"=>420,
			"height"=>16
		],
		(object)[
			"type"=>"text",
			"text"=>"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an",
			"family"=>"Lantinghei SC Demibold",
			"size"=>5,
			"alignment"=>1,
			"color"=>"ffffff",
			"x"=>420/2,
			"y"=>10
		],
		(object)[
			"type"=>"text",
			"text"=>"Add:No.9 West Section of the South 2nd Ring.LianHu District.Xi'an",
			"family"=>"Lantinghei SC Demibold",
			"size"=>5,
			"alignment"=>1,
			"color"=>"ffffff",
			"x"=>420/2,
			"y"=>297-6
		],
		(object)[
			"type"=>"svgfile",
			"filename"=>"logo.svg",
			"x"=>(420-50)/2,
			"y"=>30,
			"width"=>50,
			"height"=>37.544
		],
		(object)[
			"type"=>"text",
			"text"=>"YJBeetle",
			"family"=>"Yuanti SC",
			"size"=>20,
			"alignment"=>1,
			"color"=>"686767",
			"x"=>420/2,
			"y"=>100
		],
		(object)[
			"type"=>"text",
			"text"=>"测试中文",
			"family"=>"Lantinghei SC Extralight",
			"size"=>20,
			"alignment"=>1,
			"color"=>"686767",
			"x"=>420/2,
			"y"=>120
		],

		(object)[
			"type"=>"rectangle"
		],
		(object)[
			"type"=>"text"
		],
		(object)[
			"type"=>"svgfile"
		],
		(object)[
			"type"=>"pngfile"
		]
	]
];
echo "=========\n";
echo json_encode($data)."\n";
echo "=========\n";
echo "=========\n";
echo addslashes(json_encode($data))."\n";
echo "=========\n";
