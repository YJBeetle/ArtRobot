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

//解析json数据
$json=json_decode($jsondata);

//处理json数据
if (@$_GET['type'])
{
    switch (strtoupper($_GET['type']))
    {
        case 'PDF':
            $json->type='PDF';
            break;
        case 'SVG':
            $json->type='SVG';
            break;
        case 'PNG':
            $json->type='PNG';
            break;
        default:
    }
}
if (@$_GET['ppi'])
{
    $json->ppi=intval($_GET['ppi']);
}
if (@$_GET['unit'])
{
    $json->unit=$_GET['unit'];
}

//编码json数据
$jsonrun=json_encode($json);

//写入json文件
$filename=$TMPDIR.'/run.json';
$file = fopen($filename, "w") or die("Unable to open file!");
fwrite($file,$jsonrun);
fclose($file);

//$cmdline
$cmdline=dirname(__FILE__)."/exec/Art_robot \"$filename\"";

//设置文件类型
switch (strtoupper($json->type))
{
    case 'PDF':
        header('Content-type: application/pdf');
        break;
    case 'SVG':
        header('Content-type: image/svg+xml');
        break;
    case 'PNG':
        header('Content-type: image/png');
        break;
    default:
        header('Content-type: application/octet-stream');
}

//设置文件名
//date_default_timezone_set('Asia/Shanghai');
header('Content-Disposition: attachment;filename=MYFO-Media-'.date('y-m-d_h-i-s',time()).'.'.strtolower($json->type));

//output
passthru($cmdline);

//clear
unlink($filename);
//unset($_SESSION['jsondata']);
