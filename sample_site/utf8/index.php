<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<html>
  <head>
  </head>
  <body>
<center>
絵文字表示ﾃｽﾄ(UTF-8)
</center>
<hr>
<a href="post1_utf8.php" accesskey="2">絵文字入力のﾃｽﾄ(POST)</a><br>
<a href="get1_utf8.php" accesskey="3">絵文字入力のﾃｽﾄ(GET)</a><br>
<hr>
<a href="all-16.php" accesskey="4">16進参照文字列絵文表示ﾃｽﾄ</a><br>
<a href="all-10.php" accesskey="5">10進参照文字列絵文表示ﾃｽﾄ</a><br>
<a href="all-2.php"  accesskey="6">ﾊﾞｲﾅﾘ絵文表示ﾃｽﾄ</a><br>
<hr>
<a href="../2.php" accesskey="1">戻る</a><br>
<a href="../1.php" accesskey="0">ﾄｯﾌﾟへ</a><br>
<hr>
  </body>
</html>
