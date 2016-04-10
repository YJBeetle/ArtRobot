<?php

$DIR="/Volumes/Data0/YJBeetle/Work/Art_robot/Art_robot/file/";

$filename=$DIR."json.txt";
$file = fopen($filename, "r") or die("Unable to open file!");
$jsondata=fread($file,filesize($filename));
fclose($file);

//run
$return=exec('cd '.$DIR.';./Art_robot "'.addslashes($jsondata).'"')."\n";



?>



<!doctype html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Art robot测试页面</title>
    <style type="text/css">
        #textarea {
            width: 100%;
        }
        #textarea2 {
            width: 100%;
        }
    </style>
</head>

<body>
<h1>Art robot测试页面</h1>
<h3>设置</h3>
<p>&nbsp;</p>
<h3>输出预览</h3>
<p>&nbsp;</p>
<h3>调试输出</h3>
<p>
    json
    <textarea name="textarea" rows="30" id="textarea"><?php echo $jsondata;?></textarea>
    <br>
    stdout
    <textarea name="textarea2" id="textarea2"><?php echo $return;?></textarea>
</p>
</body>
</html>
