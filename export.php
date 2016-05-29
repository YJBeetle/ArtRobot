<?php
/**
 * Created by PhpStorm.
 * User: YJBeetle
 * Date: 16/5/25
 * Time: 下午4:33
 */
//init
session_start();

$TMPDIR=dirname(__FILE__).'/tmp';
$outfile=$TMPDIR.'/exportdata';

//获取数据
//$jsondata=$GLOBALS['HTTP_RAW_POST_DATA'];
$jsondata=$_SESSION['jsondata'];

//选中模板
$template = '1';

//设置工作目录
$dir = "template/" . $template;
chdir($dir);

//数据处理
$data=json_decode($jsondata);
$data->outfile=$outfile;

//编码并写入json文件
$jsonrun=json_encode($data);
$filename=$TMPDIR.'/run.json';
$file = fopen($filename, "w") or die("Unable to open file!");
fwrite($file,$jsonrun);
fclose($file);

//run
$cmdline=dirname(__FILE__).'/exec/Art_robot -f "'.$TMPDIR.'/run.json'.'" 2>&1';
exec($cmdline);
//passthru($cmdline);

header('Content-type: image/svg+xml');

$file = fopen($outfile, "rb");
echo fread($file,filesize($outfile));
fclose($file);
