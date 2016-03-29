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
			"type"=>"svg",
			"filename"=>"bg-veins.svg"
		],
		(object)[
			"type"=>"png",
			"filename"=>"1.png"
		],
		(object)[
			"type"=>"text",
			"text"=>"YJBeetle"
		]
	]
];
echo "=========\n";
echo json_encode($data)."\n";
echo "=========\n";
echo "=========\n";
echo addslashes(json_encode($data))."\n";
echo "=========\n";
