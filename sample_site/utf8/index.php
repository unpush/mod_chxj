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
絵文字表示ﾃｽﾄ(UTF8)
</center>
REFSTRING(16進数:UTF8)<br>
&amp;#xee98be; =&gt; [&#xee98be;]<br>
&amp;#xee9d97; =&gt; [&#xee9d97;]<br>
REFSTRING(10進数:UTF8)<br>
&amp;#15636670; =&gt; [&#15636670;]<br>
&amp;#15637911; =&gt; [&#15637911;]<br>
REFSTRING(16進数:UNICODE)<br>
&amp;#xe63e; =&gt; [&#xe63e;]<br>
&amp;#xe757; =&gt; [&#xe757;]<br>
REFSTRING(10進数:UNICODE)<br>
&amp;#58942; =&gt; [&#58942;]<br>
&amp;#59223; =&gt; [&#59223;]<br>
2ﾊﾞｲﾄｺｰﾄﾞ<br>
&amp;#xe63e; =&gt; []<br>
&amp;#xe757; =&gt; []<br>
<hr>
<a href="post1_utf8.php" accesskey="2">絵文字入力のﾃｽﾄ</a>
<hr>
<a href="../2.php" accesskey="1">戻る</a><br>
<a href="../1.php" accesskey="0">ﾄｯﾌﾟへ</a><br>
  </body>
</html>
