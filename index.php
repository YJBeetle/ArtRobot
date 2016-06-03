<?php
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

if(@$_POST['submit'])
{
    $p=array();
    $r=array();
    foreach($_POST['TEXT'] as $v)
    {
        $p[]="/\<.*".$v[0].".*\>/";
        $r[]=$v[1];
    }
    $jsonrun=preg_replace($p,$r,$jsontext);
    $_SESSION['jsondata']=$jsonrun;
}
?>
<!doctype html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Art robot测试页面</title>
</head>

<body>
<h1>Art robot测试页面-商务水牌</h1>
<form name="form" method="post" enctype="multipart/form-data">
    <h3>设置</h3>
    <?php
    preg_match_all("/\<.*\>/",$jsontext,$a);
    foreach ($a[0] as $k=>$v)
    {
        $b=explode('|',substr($v,1,-1));
        if($b[0]=='TEXT')
        {
            $title=$b[1];
            $value=$b[2];
            if(@$_POST['TEXT'][$k][1])$value=$_POST['TEXT'][$k][1];
            echo "<p>$title<input type=\"text\" name=\"TEXT[$k][1]\" value=\"$value\"><input type=\"hidden\" name=\"TEXT[$k][0]\" value=\"$title\"></p>";
        }
    }
    ?>
    <p><input type="submit" name="submit" value="提交"></p>
</form>
<?php
if(@$_POST['submit']) {
    ?>
    <h3>输出预览</h3>
    <p><img src="export.php?type=svg&unit=px"></p>
    <h3>下载</h3>
    <p><a href="export.php?type=pdf&unit=mm">下载PDF版本</a></p>
    <p><a href="export.php?type=png&unit=mm">下载PNG版本</a></p>
    <p><a href="export.php?type=png&unit=mm&ppi=300">下载PNG版本（300ppi）</a></p>
    <?php
}
?>
</body>
</html>
