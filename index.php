<?php
//init
session_start();

if(@$_POST['submit']) {
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

//数据处理
    $json = json_decode($jsontext);
    $json->type="SVG";

//设置文字内容
    /*
    $json->draw[0][7]->text = $_POST['text'];
    $json->draw[0][8]->text = $_POST['text2'];
    $json->draw[0][9]->text = '会议时间:' . $_POST['date'];
    $json->draw[0][10]->text = '会议地点:' . $_POST['where'];
    $json->draw[0][11]->text = 'Date:' . $_POST['date-en'];
    $json->draw[0][12]->text = 'Location:' . $_POST['where-en'];
    */
    $json->draw[7]->text = $_POST['text'];
    $json->draw[8]->text = $_POST['text2'];
    $json->draw[9]->text = '会议时间:' . $_POST['date'];
    $json->draw[10]->text = '会议地点:' . $_POST['where'];
    $json->draw[11]->text = 'Date:' . $_POST['date-en'];
    $json->draw[12]->text = 'Location:' . $_POST['where-en'];

//编码并写入
    $jsonrun = json_encode($json);
    $_SESSION['jsondata']=$jsonrun;
}
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
<h1>Art robot测试页面-商务水牌</h1>
<form name="form" method="post" enctype="multipart/form-data">
    <h3>设置</h3>
    <p>标题:<input type="text" name="text" value="会议名称"></p>
    <p>标题:<input type="text" name="text2" value="会议名称第二行"></p>
    <p>会议时间:<input type="text" name="date" value="2016年1月1日"></p>
    <p>会议时间（英文）:<input type="text" name="date-en" value="January 1 2016"></p>
    <p>会议地点:<input type="text" name="where" value="会议室1"></p>
    <p>会议地点（英文）:<input type="text" name="where-en" value="room 1"></p>
    <p><input type="submit" name="submit" value="提交"></p>
</form>
<?php
if(@$_POST['submit']) {
    ?>
    <h3>输出预览</h3>
    <p><img src="export.php"></p>
    <h3>下载</h3>
    <p><a href="export.php?type=pdf">下载PDF版本</a></p>
    <p><a href="export.php?type=png">下载PNG版本</a></p>
    <?php
    /*
    <h3>调试输出</h3>
    <p>
        模板json数据
        <textarea name="textarea" rows="10" id="textarea"><?php echo $jsontext; ?></textarea>
        处理后数据结构
        <textarea name="textarea" rows="10" id="textarea"><?php echo print_r($json); ?></textarea>
        生成json数据
        <textarea name="textarea" rows="10" id="textarea"><?php echo $jsonrun; ?></textarea>
    </p>
*/
    ?>
    <?php
}
?>
</body>
</html>
