<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<html>
  <head>
    <title>mod_chxj�Î��Ď�����</title>
  </head>
  <body>
    <center>
      <img src="./img/logo">
      <hr>
      <br>
      <font color="#FF0000">�܎����Ĥ�mod_chxj�η��Ӹ����Î��Ď����ĤǤ�</font>���<br>
      <font color="#FF0000">�Î��ĎÎ��ĎÎ���</font>������
      <br>
      <br>
    </center>
    <hr>
        ��ݎՎ�����ID<br>
        <?php echo $_POST["id"]; ?>�����Ϥ��ޤ����͏��<br>
        ��ގʎߎ��܎��Ď�<br>
        <?php echo $_POST["pass"]; ?>�����Ϥ��ޤ����͏��<br>
    <br>
    <br>
    ���<a href="3.php" accesskey="1">���ώ̎����ѤΎÎ���</a><br>
    ���<a href="4.php" accesskey="2">���ގ��ݎێ��ĎޤΎÎ���</a><br>
    ���<a href="qrcode.php" accesskey="3">QRCODE�ΎÎ���</a><br>
    ���<a href="extend_tag.php" accesskey="4">��ĥ�����ΎÎ���</a><br>
    ���<a href="image.php" accesskey="5">ưŪ�����ΎÎ���</a><br>
    ���<a href="d_qrc.php" accesskey="6">ưŪQRCODE�ΎÎ���</a><br>
    ���<a href="sjis/index.php" accesskey="7">��ʸ���ΎÎ���(SJIS)</a><br>
    ���<a href="euc/index.php" accesskey="8">��ʸ���ΎÎ���(EUCJP)</a><br>
    ���<a href="utf8/index.php" accesskey="9">��ʸ���ΎÎ���(UTF8)</a><br>
    ���<a href="1.php" accesskey="0">�Ď��̎ߤ����</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
