<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<html>
  <head>
  </head>
  <body>
<?php
  $data = $_GET[""];
?>
<center>
絵文字入力のﾃｽﾄ(EUCJPでGET)
</center>
<hr>
    <form name="" method="get" action="get1_utf8.php">
      <input type="text" name="" value="<?=${data}?>" />
      <input type="submit" value="" />
    </form>
ｹﾞｯﾄされた値は、、、<br>
[
<?php
  echo "HEX:[" . bin2hex($data) . "]<br>";
  echo "BIN:[" . $data . "]<br>";
?>
です。<br>

<hr>

<br>
<a href="index.php" accesskey="1">戻る</a><br>
  </body>
</html>
