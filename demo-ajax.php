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
$template = '1';

//设置工作目录
$dir = "template/" . $template;
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