<?php
/**
 * Created by PhpStorm.
 * User: YJBeetle
 * Date: 16/6/7
 * Time: 下午6:38
 */


//init
session_start();
$return=array();
$return['status']=0;

//常量
//$TMPDIR=sys_get_temp_dir();
$TMPDIR = dirname(__FILE__) . '/tmp';
if (!is_dir($TMPDIR)) mkdir($TMPDIR, 0777);

//选中模板
if (@$_GET['template'])
{
    $template = $_GET['template'];

    //设置工作目录
    $dir = "template/" . $template;
    if(!is_dir($dir))
    {
        $return['status']=2;
        $return['message']='模板不存在';

    }
    else
    {
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
                $p[]="/\<TEXT\|".$v[0]."\|.*\>/";
                //$p[]="/\<.*".$v[0].".*\>/";
                $r[]=$v[1];
            }
        }
        if(@$_POST['IMG'])
        {
            foreach($_POST['IMG'] as $v)
            {
                $filename=0;
                //$file = fopen($filename, "w") or die("Unable to open file!");
                //$p[]="/\<TEXT\|".$v[0]."\|.*\>/";
                //$r[]=$v[1];
            }
        }
        $jsonrun=preg_replace($p,$r,$jsontext);
        $_SESSION['jsondata']=$jsonrun;

        $return['status']=0;
        $return['message']='绘制完毕';
        $json=json_decode($jsonrun);
        $return['height']=$json->height;
        $return['width']=$json->width;
        $return['pagecount']=is_array($json->draw[0])?count($json->draw):1;
    }
}
else
{
    $return['status']=1;
    $return['message']='未指定模板';
}

echo json_encode($return);
