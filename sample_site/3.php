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
    入力ﾌｫｰﾑのﾃｽﾄ1
    </center>
    <hr>
      <form name="a" method="post" action="3-1.php">
        ﾃｷｽﾄﾎﾞｯｸｽ(istyle=1)<br>
        <input type="text" name="i1" maxlength="10" size="10" istyle="1"><br>
        ﾃｷｽﾄﾎﾞｯｸｽ(istyle=2)<br>
        <input type="text" name="i2" maxlength="10" size="10" istyle="2"><br>
        ﾃｷｽﾄﾎﾞｯｸｽ(istyle=3)<br>
        <input type="text" name="i3" maxlength="10" size="10" istyle="3"><br>
        ﾃｷｽﾄﾎﾞｯｸｽ(istyle=4)<br>
        <input type="text" name="i4" maxlength="10" size="10" istyle="4"><br>
        ﾃｷｽﾄﾎﾞｯｸｽ(istyleなし)<br>
        <input type="text" name="i5" maxlength="10" size="10"><br>
        ﾊﾟｽﾜｰﾄﾞ<br>
        <input type="password" name="pass" maxlength="10" size="10"><br>
        ﾗｼﾞｵﾎﾞﾀﾝ<br>
        <input type="radio" name="r1" value="1">1を選択<br>
        <input type="radio" name="r1" value="2">2を選択<br>
        <input type="radio" name="r1" value="3">3を選択<br>
        ﾁｪｯｸﾎﾞｯｸｽ<br>
        <input type="checkbox" name="c1[0]" value="1">1を選択<br>
        <input type="checkbox" name="c1[1]" value="2">2を選択<br>
        <input type="checkbox" name="c1[2]" value="3">3を選択<br>
        ｾﾚｸﾄﾎﾞｯｸｽ<br>
        <select name="s1">
          <option value=" "> </option>
          <option value="1">1を選択</option>
          <option value="2">2を選択</option>
          <option value="3">3を選択</option>
        </select><br>
        <div align="right"><input type="submit" name="submit" value="  送信1  "></div>
        <input type="submit" name="submit" value="  送信2  ">
      </form>
    <br>
    <br>
    ���<a href="1.php" accesskey="0">ﾄｯﾌﾟへ戻る</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
