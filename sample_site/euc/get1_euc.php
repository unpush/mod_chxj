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
  $data = $_GET["ü¡"];
  if (strlen($data) == 0) {
    /* for PC */
    $data = $_GET["&#58942;"];
  }
?>
<center>
³¨Ê¸»úÆþÎÏ¤ÎŽÃŽ½ŽÄ(EUCJP¤ÇGET)
</center>
<hr>
    <form name="ü¡" method="get" action="get1_euc.php">
      <input type="text" name="ü¡" value="<?=${data}?>" />
      <input type="submit" value="þþ" />
    </form>
Ž¹ŽÞŽ¯ŽÄ¤µ¤ì¤¿ÃÍ¤Ï¡¢¡¢¡¢<br>
[
<?php
  echo "HEX:[" . bin2hex($data) . "]<br>";
  echo "BIN:[" . $data . "]<br>";
?>
¤Ç¤¹¡£<br>

<hr>

<br>
ýç<a href="index.php" accesskey="1">Ìá¤ë</a><br>
  </body>
</html>
