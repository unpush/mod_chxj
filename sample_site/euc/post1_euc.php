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
  $data = $_POST["���"];
?>
<center>
��ʸ�����ϤΎÎ���(EUCJP)
</center>
<hr>
    <form name="���" method="post" action="post1_euc.php">
      <input type="text" name="���" value="<?=${data}?>" />
      <input type="submit" value="���" />
    </form>
�Ύߎ��Ĥ��줿�ͤϡ�����<br>
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
