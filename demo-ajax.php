<?php
/**
 * Created by PhpStorm.
 * User: YJBeetle
 * Date: 16/6/7
 * Time: 下午6:38
 */

//init
session_start();

//常量
//$TMPDIR=sys_get_temp_dir();
$TMPDIR = dirname(__FILE__) . '/tmp';
if (!is_dir($TMPDIR)) mkdir($TMPDIR, 0777);

//选中模板
if (@$_GET['template'])
    $template = $_GET['template'];
else
{
    echo '未指定模板';
    exit(1);
}

//设置工作目录
$dir = "template/" . $template;
if(!is_dir($dir))
{
    echo '模板不存在';
    exit(2);
}
chdir($dir);

//读取json文件
$filename = "data.json";
$file = fopen($filename, "r") or die("Unable to open file!");
$jsontext = fread($file, filesize($filename));
fclose($file);

//POST处理
$p=array();
$r=array();
if(@$_POST['TEXT'])
{
    foreach($_POST['TEXT'] as $v)
    {
        $p[]="/\<.*".$v[0].".*\>/";
        $r[]=$v[1];
    }
}
$jsonrun=preg_replace($p,$r,$jsontext);
$_SESSION['jsondata']=$jsonrun;

echo '绘制完毕';