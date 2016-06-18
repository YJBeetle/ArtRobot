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
    $template = 'default';

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
        showloading();
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
                    showerror();
                }
                else
                {
                    $("#showinfo").html(ret.message);

                    count=ret.pagecount;
                    fwidth=$("#canvas")[0].width;
                    fheight=$("#canvas")[0].height;
                    cwidth = ret.width;
                    cheight = ret.height;

                    scale=fwidth/cwidth;

                    width=cwidth*scale;
                    height=cheight*scale;
                    x=0;
                    y=0;

                    drawclear();
                    if(count>1)
                    {
                        $("#canvas")[0].height=height*count;
                        for(var i=0;i<count;i++)
                        {
                            y=height*i;
                            draw("export.php?template=<?php echo $template;?>&type=svg&unit=px&pageonly="+i.toString(),x,y,width,height);
                        }
                    }
                    else
                        draw("export.php?template=<?php echo $template;?>&type=svg&unit=px");
                }
            },
            error: function() {
                $("#showinfo").html("处理发生了错误");
                showerror();
            }
        });
        //$("#showinfo").html(htmlobj.responseText);
    }

    function draw(url,x,y,width,height){
        $.ajax({
            url:url,
            //async:true,
            dataType: 'text',
            success: function(data) {
                var img = new Image();
                img.onload = function() {
                    if(!(width&&height))
                    {
                        fwidth=$("#canvas")[0].width;
                        fheight=$("#canvas")[0].height;
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
                    }
                    var ctx = $("#canvas")[0].getContext("2d");
                    ctx.clearRect(x, y, width, height);
                    ctx.drawImage(img, x, y, width, height);
                    showcanvas();
                    $("#showinfo").html("绘制完成");
                };
                img.src = 'data:image/svg+xml;base64,' + btoa(data);
            },
            error: function() {
                $("#showinfo").html("处理发生了错误");
            }
        });
    }

    function drawclear() {
        var ctx = $("#canvas")[0].getContext("2d");
        ctx.clearRect(0,0,$("#canvas")[0].width,$("#canvas")[0].height);
    }

    function showcanvas() {
        $("#canvas").css("display","block");
        $("#loading").css("display","none");
        $("#error").css("display","none");
    }

    function showloading() {
        $("#canvas").css("display","none");
        $("#loading").css("display","block");
        $("#error").css("display","none");
    }

    function showerror() {
        $("#canvas").css("display","none");
        $("#loading").css("display","none");
        $("#error").css("display","block");
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
        elseif($b[0]=='IMG')
        {
            $title=$b[1];
            echo "$title<input type=\"file\" name=\"IMG[$k][1]\"><input type=\"hidden\" name=\"IMG[$k][0]\" value=\"$title\">";
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
    <image id="loading" src="image/loading.svg" style="display: none;"></image>
    <image id="error" src="image/error.svg" style="display: none;"></image>
</div>
<button onclick="$('#canvas')[0].width=$('#canvasdiv').width();$('#canvas')[0].height=$('#canvasdiv').height();sub()">1X</button>
<button onclick="$('#canvas')[0].width=$('#canvasdiv').width()*2;$('#canvas')[0].height=$('#canvasdiv').height()*2;sub()">2X</button>
<button onclick="$('#canvas')[0].width=$('#canvasdiv').width()*4;$('#canvas')[0].height=$('#canvasdiv').height()*4;sub()">4X</button>
<h3>下载</h3>
<p>
    <a href="export.php?template=<?php echo $template;?>&type=pdf&unit=mm">下载PDF版本</a>
    <br>
    <a href="export.php?template=<?php echo $template;?>&type=png&unit=mm&pageonly=0">下载PNG版本</a>
    <br>
    <a href="export.php?template=<?php echo $template;?>&type=png&unit=mm&ppi=300&pageonly=0">下载PNG版本（300ppi）</a>
</p>
</body>
</html>
