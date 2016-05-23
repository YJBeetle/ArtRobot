<?php

header('Content-type: image/svg+xml');
$filename=sys_get_temp_dir().'/out.svg';

$file = fopen($filename, "rb");
$svg=fread($file,filesize($filename));
fclose($file);

echo $svg;
