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
絵文字表示ﾃｽﾄ(EUCJP)
</center>
<hr>
REFSTRING(16進数)<br>
&amp;#x8ffca1; =&gt; [&#x8ffca1;]<br>
&amp;#x8ffefe; =&gt; [&#x8ffefe;]<br>
REFSTRING(10進数)<br>
&amp;#9436321; =&gt; [&#9436321;]<br>
&amp;#9436926; =&gt; [&#9436926;]<br>
2バイトコード<br>
&amp;#x8ffca1; =&gt; [���]<br>
&amp;#x8ffefe; =&gt; [���]<br>
<hr>
���<a href="post1_euc.php" accesskey="2">絵文字入力のﾃｽﾄ</a>
<hr>
���<a href="../2.php" accesskey="1">戻る</a><br>
���<a href="../1.php" accesskey="0">ﾄｯﾌﾟへ</a><br>
  </body>
</html>
