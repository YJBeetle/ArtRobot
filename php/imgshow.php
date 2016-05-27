<?php

header('Content-type: image/svg+xml');

$TMPDIR=dirname(__FILE__).'/tmp';

$filename=$TMPDIR.'/out.svg';

$file = fopen($filename, "rb");
echo fread($file,filesize($filename));
fclose($file);
