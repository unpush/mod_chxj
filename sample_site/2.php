<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<html>
  <head>
    <title>mod_chxjŽÃŽ½ŽÄŽ»Ž²ŽÄ</title>
  </head>
  <body>
    <center>
      <img src="./img/logo">
      <hr>
      <br>
      <font color="#FF0000">ËÜŽ»Ž²ŽÄ¤Ïmod_chxj¤Î·ÈÂÓ¸þ¤±ŽÃŽ½ŽÄŽ»Ž²ŽÄ¤Ç¤¹</font>þ©<br>
      <font color="#FF0000">ŽÃŽ½ŽÄŽÃŽ½ŽÄŽÃŽ½ŽÄ</font>þ«þª
      <br>
      <br>
    </center>
    <hr>
        ýÝŽÕŽ°Ž»ŽÞID<br>
        <?php echo $_POST["id"]; ?>¤òÆþÎÏ¤·¤Þ¤·¤¿¤Íþ©<br>
        ýÞŽÊŽßŽ½ŽÜŽ°ŽÄŽÞ<br>
        <?php echo $_POST["pass"]; ?>¤òÆþÎÏ¤·¤Þ¤·¤¿¤Íþ©<br>
    <br>
    <br>
    ýç<a href="3.php" accesskey="1">ÆþÎÏŽÌŽ«Ž°ŽÑ¤ÎŽÃŽ½ŽÄ</a><br>
    ýè<a href="4.php" accesskey="2">ŽÀŽÞŽ³ŽÝŽÛŽ°ŽÄŽÞ¤ÎŽÃŽ½ŽÄ</a><br>
    ýé<a href="qrcode.php" accesskey="3">QRCODE¤ÎŽÃŽ½ŽÄ</a><br>
    ýê<a href="extend_tag.php" accesskey="4">³ÈÄ¥¥¿¥°¤ÎŽÃŽ½ŽÄ</a><br>
    ýë<a href="image.php" accesskey="5">Æ°Åª²èÁü¤ÎŽÃŽ½ŽÄ</a><br>
    ýì<a href="d_qrc.php" accesskey="6">Æ°ÅªQRCODE¤ÎŽÃŽ½ŽÄ</a><br>
    ýí<a href="sjis/index.php" accesskey="7">³¨Ê¸»ú¤ÎŽÃŽ½ŽÄ(SJIS)</a><br>
    ýî<a href="euc/index.php" accesskey="8">³¨Ê¸»ú¤ÎŽÃŽ½ŽÄ(EUCJP)</a><br>
    ýï<a href="utf8/index.php" accesskey="9">³¨Ê¸»ú¤ÎŽÃŽ½ŽÄ(UTF8)</a><br>
    ýð<a href="1.php" accesskey="0">ŽÄŽ¯ŽÌŽß¤ØÌá¤ë</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
