<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<html>
  <head>
    <title>mod_chxjﾃｽﾄｻｲﾄ</title>
  </head>
  <body>
    <center>
      ﾀﾞｳﾝﾛｰﾄﾞと画像変換のﾃｽﾄ&#63911;<br>
      <br>
      <font color="#FF0000">壁紙１</font>
      <hr>
      <br>
    <img src="./img/sample/65?Mode=thumbnail"><br>
<chxj:if lang="chtml" lang="jhtml">
    <a href="./img/sample/65?Mode=WP">ﾀﾞｳﾝﾛｰﾄﾞ</a>
</chxj:if>
<chxj:if lang="xhtml">
    <a href="./img/sample/65?Mode=EZGET">ﾀﾞｳﾝﾛｰﾄﾞ</a>
</chxj:if>
<chxj:if lang="hdml">
    <CENTER><a task=GOSUB DEST="./img/sample/65?Mode=EZGET">ﾀﾞｳﾝﾛｰﾄﾞ</a><BR>
</chxj:if>
      <br>
    </center>
    <hr>
    &#63888;<a href="1.php" accesskey="0">ﾄｯﾌﾟへ戻る</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
