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
  $data = $_GET["���"];
  if (strlen($data) == 0) {
    /* for PC */
    $data = $_GET["&#58942;"];
  }
?>
<center>
��ʸ�����ϤΎÎ���(EUCJP��GET)
</center>
<hr>
    <form name="���" method="get" action="get1_euc.php">
      <input type="text" name="���" value="<?=${data}?>" />
      <input type="submit" value="���" />
    </form>
���ގ��Ĥ��줿�ͤϡ�����<br>
[
<?php
  echo "HEX:[" . bin2hex($data) . "]<br>";
  echo "BIN:[" . $data . "]<br>";
?>
�Ǥ���<br>

<hr>

<br>
���<a href="index.php" accesskey="1">���</a><br>
  </body>
</html>
