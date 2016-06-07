<?php
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
    $template = '1';

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

if(@$_POST['submit'])
{
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
}
?>
<!doctype html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Art robot测试页面</title>
    <script src="js/jquery-2.2.4.js"></script>
</head>

<body>
<script>
    function sub() {
        $("#showinfo").html("正在绘制");
        //$("#showsvg").attr("src","");
        htmlobj=$.ajax({
            url:"demo-ajax.php?template=<?php echo $template;?>",
            //async:true,
            type: "POST",
            cache: false,
            data: $("#form").serialize(),
            success: function(result) {
                $("#showinfo").html(result);
                draw("export.php?template=<?php echo $template;?>&type=svg&unit=px&pageonly=1");
            },
            error: function() {
                $("#showinfo").html("处理发生了错误");
            }
        });
        //$("#showinfo").html(htmlobj.responseText);
    }

    function draw(imgurl){
        htmlobj=$.ajax({
            url:imgurl,
            //async:true,
            dataType: 'text',
            success: function(result) {
                var data=result;
                var img = new Image();
                var DOMURL = self.URL || self.webkitURL || self;
                var svg = new Blob([data], {type: "image/svg+xml;charset=utf-8"});
                var url = DOMURL.createObjectURL(svg);
                img.onload = function() {
                    var canvas = document.getElementById("canvas");
                    var ctx = canvas.getContext("2d");
                    var width = img.width;
                    var height = img.height;
                    var x = 0;
                    var y = 0;
                    ctx.drawImage(img, x, y, width, height);
                    DOMURL.revokeObjectURL(url);
                };
                img.src = url;
            },
            error: function() {
                $("#showinfo").html("处理发生了错误");
            }
        });
    }

    $(document).ready(function () {
        sub();
    })
</script>
<h1>Art robot测试页面-商务水牌</h1>
<form id="form" method="post" enctype="multipart/form-data">
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
            echo "$title<input type=\"text\" name=\"TEXT[$k][1]\" value=\"$value\"><input type=\"hidden\" name=\"TEXT[$k][0]\" value=\"$title\">";
            echo '<br>';
        }
    }
    ?>
</form>
<input type="button" value="提交" onclick="sub()">
<button onclick="draw('image/loading.svg')">draw</button>
<h3>信息</h3>
<strong><pre id="showinfo">请点击提交以预览</pre></strong>
<h3>输出预览</h3>
<canvas id="canvas" width="500" height="500"></canvas>
<h3>下载</h3>
<p>
    <a href="export.php?template=<?php echo $template;?>&type=pdf&unit=mm">下载PDF版本</a>
    <br>
    <a href="export.php?template=<?php echo $template;?>&type=png&unit=mm&pageonly=1">下载PNG版本</a>
    <br>
    <a href="export.php?template=<?php echo $template;?>&type=png&unit=mm&ppi=300&pageonly=1">下载PNG版本（300ppi）</a>
</p>
</body>
</html>
