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
  $data = $_POST["��"];
  if (strlen($data) == 0) {
    /* for PC */
    $data = $_POST["&#58942;"];
  }
?>
<center>
�G�������͂�ý�(SJIS��POST)
</center>
<hr>
    <form name="��" method="post" action="post1_sjis.php">
      <input type="text" name="��" value="<?=${data}?>" />
      <input type="submit" value="��" />
    </form>
�߽Ă��ꂽ�l�́A�A�A<br>
<?php
  echo "HEX:[" . bin2hex($data) . "]<br>";
  echo "BIN:[" . $data . "]<br>";
?>
�ł��B<br>

<hr>

<br>
��<a href="index.php" accesskey="1">�߂�</a><br>
  </body>
</html>
