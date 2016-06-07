<?php
/**
 * Created by PhpStorm.
 * User: YJBeetle
 * Date: 16/6/7
 * Time: 下午9:17
 */
?>
<!doctype html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Art robot测试页面</title>
    <script src="js/jquery-2.2.4.js"></script>
</head>

<body>
<h1>选择模板</h1>
<?php
$directory='template';
$files = array();
if(is_dir($directory)) {
    if($files = scandir($directory)) {
        $files = array_slice($files,2);
    }
}
foreach ($files as $file)
{
    if (is_dir($directory.'/'.$file))
        echo "<p><a href='demo.php?template=$file'>$file</a></p>";
}
?>
</body>
</html>
