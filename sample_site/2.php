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
      <img src="./img/logo">
      <hr>
      <br>
      <font color="#FF0000">本ｻｲﾄはmod_chxjの携帯向けﾃｽﾄｻｲﾄです</font>���<br>
      <font color="#FF0000">ﾃｽﾄﾃｽﾄﾃｽﾄ</font>������
      <br>
      <br>
    </center>
    <hr>
        ���ﾕｰｻﾞID<br>
        <?php echo $_POST["id"]; ?>を入力しましたね���<br>
        ���ﾊﾟｽﾜｰﾄﾞ<br>
        <?php echo $_POST["pass"]; ?>を入力しましたね���<br>
    <br>
    <br>
    ���<a href="3.php" accesskey="1">入力ﾌｫｰﾑのﾃｽﾄ</a><br>
    ���<a href="4.php" accesskey="2">ﾀﾞｳﾝﾛｰﾄﾞのﾃｽﾄ</a><br>
    ���<a href="qrcode.php" accesskey="3">QRCODEのﾃｽﾄ</a><br>
    ���<a href="extend_tag.php" accesskey="4">拡張タグのﾃｽﾄ</a><br>
    ���<a href="image.php" accesskey="5">動的画像のﾃｽﾄ</a><br>
    ���<a href="d_qrc.php" accesskey="6">動的QRCODEのﾃｽﾄ</a><br>
    ���<a href="sjis/index.php" accesskey="7">絵文字のﾃｽﾄ(SJIS)</a><br>
    ���<a href="euc/index.php" accesskey="8">絵文字のﾃｽﾄ(EUCJP)</a><br>
    ���<a href="utf8/index.php" accesskey="9">絵文字のﾃｽﾄ(UTF8)</a><br>
    ���<a href="1.php" accesskey="0">ﾄｯﾌﾟへ戻る</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
