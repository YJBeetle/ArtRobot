<?php
/**
 * Created by PhpStorm.
 * User: YJBeetle
 * Date: 16/5/25
 * Time: 下午4:33
 */
//init
error_reporting(0);//禁用错误报告
session_start();

$TMPDIR=dirname(__FILE__).'/tmp';

//获取数据
//$jsondata=$GLOBALS['HTTP_RAW_POST_DATA'];
$jsondata=$_SESSION['jsondata'];

//选中模板
$template = '1';

//设置工作目录
$dir = "template/" . $template;
chdir($dir);

//写入json文件
$filename=$TMPDIR.'/run.json';
$file = fopen($filename, "w") or die("Unable to open file!");
fwrite($file,$jsondata);
fclose($file);

//$cmdline
$cmdline=dirname(__FILE__)."/exec/Art_robot \"$filename\"";

//识别并设置文件类型
$json=json_decode($jsondata);
switch (strtoupper($json->type))
{
    case "PDF":
        header('Content-type: application/pdf');
        break;
    case "SVG":
        header('Content-type: image/svg+xml');
        break;
    case "PNG":
        header('Content-type: image/png');
        break;
    default:
}

//设置文件名
//date_default_timezone_set('Asia/Shanghai');
header('Content-Disposition: attachment;filename=MYFO-Media-'.date('y-m-d_h-i-s',time()).'.'.$json->type);

//output
passthru($cmdline);

//clear
unlink($filename);