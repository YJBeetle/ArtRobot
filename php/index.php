<?php

//设置工作目录
$DIR="../file/";
chdir($DIR);

//读取json文件
$filename="json.txt";
$file = fopen($filename, "r") or die("Unable to open file!");
$jsontext=fread($file,filesize($filename));
fclose($file);

//数据处理
$json=json_decode($jsontext);
$outfile=sys_get_temp_dir().'/out.svg';
$json->outfile=$outfile;
$jsonrun=json_encode($json);

//run
$return=exec('Art_robot "'.addslashes($jsonrun).'"');



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
<p><img src="imgshow.php"></p>
<h3>调试输出</h3>
<p>
    jsontext
    <textarea name="textarea" rows="10" id="textarea"><?php echo $jsontext;?></textarea>
    json
    <textarea name="textarea" rows="10" id="textarea"><?php echo print_r($json);?></textarea>
    jsonrun
    <textarea name="textarea" rows="10" id="textarea"><?php echo $jsonrun;?></textarea>
    <br>
    stdout
    <textarea name="textarea2" id="textarea2"><?php echo $return;?></textarea>
</p>
</body>
</html>
