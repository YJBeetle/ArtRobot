<?php
/**
 * Created by PhpStorm.
 * User: YJBeetle
 * Date: 16/5/25
 * Time: 下午4:33
 */


//选中模板
$template='1';

//设置工作目录
$dir="template/".$template;
chdir($dir);

//读取json文件
$filename="data.json";
$file = fopen($filename, "r") or die("Unable to open file!");
$jsontext=fread($file,filesize($filename));
fclose($file);

//数据处理
$json=json_decode($jsontext);
$outfile=sys_get_temp_dir().'/out.svg';
$json->outfile=$outfile;

//设置文字内容
$json->draw[0][7]->text=$_GET['text'];
$json->draw[0][8]->text=$_GET['text2'];
$json->draw[0][9]->text='会议时间:'.$_GET['date'];
$json->draw[0][10]->text='会议地点:'.$_GET['where'];
$json->draw[0][11]->text='Date:'.$_GET['date'];
$json->draw[0][12]->text='Location:'.$_GET['where-en'];

//编码并写入json文件
$jsonrun=json_encode($json);
$filename=sys_get_temp_dir().'/run.json';
$file = fopen($filename, "w") or die("Unable to open file!");
fwrite($file,$jsonrun);
fclose($file);


//run
$cmdline=dirname(__FILE__).'/exec/Art_robot -f "'.sys_get_temp_dir().'/run.json'.'"';
$return=exec($cmdline);


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
<h3>输出预览</h3>
<p><img src="imgshow.php"></p>
<h3>调试输出</h3>
<p>
    模板json数据
    <textarea name="textarea" rows="10" id="textarea"><?php echo $jsontext;?></textarea>
    处理后数据结构
    <textarea name="textarea" rows="10" id="textarea"><?php echo print_r($json);?></textarea>
    生成json数据
    <textarea name="textarea" rows="10" id="textarea"><?php echo $jsonrun;?></textarea>
    command line
    <textarea name="textarea" rows="10" id="textarea"><?php echo $cmdline;?></textarea>
    <br>
    stdout
    <textarea name="textarea2" rows="5" id="textarea2"><?php echo $return;?></textarea>
</p>
</body>
</html>
