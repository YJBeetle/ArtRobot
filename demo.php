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
        draw('image/loading.svg');
        htmlobj=$.ajax({
            url:"demo-ajax.php?template=<?php echo $template;?>",
            //async:true,
            type: "POST",
            cache: false,
            data: $("#form").serialize(),
            success: function(result) {
                ret=JSON.parse(result);
                if(status)
                {
                    $("#showinfo").html(ret.message);
                    draw('image/error.svg');
                }
                else
                {
                    $("#showinfo").html(ret.message);
                    var arr=new Array();
                    for(var i=1;i<=ret.pagecount;i++)
                    {
                        arr[i-1]="export.php?template=<?php echo $template;?>&type=svg&unit=px&pageonly="+i.toString();
                    }
                    draw(arr);
                }
            },
            error: function() {
                $("#showinfo").html("处理发生了错误");
                draw('image/error.svg');
            }
        });
        //$("#showinfo").html(htmlobj.responseText);
    }

    function draw(arr){
        var ctx = $("#canvas")[0].getContext("2d");

        if(arr instanceof Array)
            length=arr.length;
        else
            length=1;
        for(i=0;i<length;i++)
        {
            if(arr instanceof Array)
                url=arr[i];
            else
                url=arr;
            var ajaxcomplete=false;
            ii=0;
            $.ajax({
                url:url,
                //async:true,
                dataType: 'text',
                success: function(result) {
                    var data=result;
                    var img = new Image();
                    var DOMURL = self.URL || self.webkitURL || self;
                    var svg = new Blob([data], {type: "image/svg+xml;charset=utf-8"});
                    var url = DOMURL.createObjectURL(svg);
                    img.onload = function() {
                        var width;
                        var height;
                        var x;
                        var y;
                        if(arr instanceof Array)
                        {
                            ffwidth=$("#canvasdiv").width();
                            ffhetght=$("#canvasdiv").height();
                            fwidth=$("#canvas").width();
                            fheight=$("#canvas").height();
                            cwidth = img.width;
                            cheight = img.height;

                            scale=fwidth/cwidth;

                            width=cwidth*scale;
                            height=cheight*scale;
                            x=0;
                            y=height*ii;

                            if(!ii)
                            {
                                $("#canvas")[0].height=cheight*length;
                                ctx.clearRect(0,0,fwidth,fheight);
                            }
                            ii++;
                        }
                        else
                        {
                            fwidth=$("#canvas").width();
                            fheight=$("#canvas").height();
                            cwidth = img.width;
                            cheight = img.height;

                            if(fwidth/fheight<cwidth/cheight)
                                scale=fwidth/cwidth;
                            else
                                scale=fheight/cheight;

                            width=cwidth*scale;
                            height=cheight*scale;
                            x=(fwidth-width)/2;
                            y=(fheight-height)/2;

                            $("#canvas")[0].height=$("#canvasdiv").height();
                            ctx.clearRect(0,0,fwidth,fheight);
                        }

                        ctx.drawImage(img, x, y, width, height);
                        DOMURL.revokeObjectURL(url);
                    };
                    img.src = url;
                },
                error: function() {
                    $("#showinfo").html("处理发生了错误");
                },
                complete: function () {
                    ajaxcomplete=true;
                }
            });
        }
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
<h3>信息</h3>
<strong><pre id="showinfo">请点击提交以预览</pre></strong>
<h3>输出预览</h3>
<div id="canvasdiv" width="500" height="500" style="width:500px; height:500px; overflow:auto">
    <canvas id="canvas" width="500" height="500"></canvas>
</div>
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
