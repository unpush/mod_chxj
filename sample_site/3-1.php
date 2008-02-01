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
    ÆþÎÏŽÌŽ«Ž°ŽÑ¤ÎŽÃŽ½ŽÄ1
    </center>
    <hr>
        <font color="#FF0000">¢£ŽÃŽ·Ž½ŽÄŽÎŽÞŽ¯Ž¸Ž½(istyle=1)</font><br>
        <?php echo $_POST["i1"] ?><br>
        <font color="#FF0000">¢£ŽÃŽ·Ž½ŽÄŽÎŽÞŽ¯Ž¸Ž½(istyle=2)</font><br>
        <?php echo $_POST["i2"] ?><br>
        <font color="#FF0000">¢£ŽÃŽ·Ž½ŽÄŽÎŽÞŽ¯Ž¸Ž½(istyle=3)</font><br>
        <?php echo $_POST["i3"] ?><br>
        <font color="#FF0000">¢£ŽÃŽ·Ž½ŽÄŽÎŽÞŽ¯Ž¸Ž½(istyle=4)</font><br>
        <?php echo $_POST["i4"] ?><br>
        <font color="#FF0000">¢£ŽÃŽ·Ž½ŽÄŽÎŽÞŽ¯Ž¸Ž½(istyle=¤Ê¤·)</font><br>
        <?php echo $_POST["i5"] ?><br>
        <font color="#FF0000">¢£ŽÊŽßŽ½ŽÜŽ°ŽÄŽÞ</font><br>
        <?php echo $_POST["pass"]; ?><br>
        <font color="#FF0000">¢£Ž×Ž¼ŽÞŽµŽÎŽÞŽÀŽÝ</font><br>
        <?php echo $_POST["r1"]; ?><br>
        <font color="#FF0000">¢£ŽÁŽªŽ¯Ž¸ŽÎŽÞŽ¯Ž¸Ž½</font><br>
        <?php 
          $c1 = $_POST["c1"]; 
          if ($c1) {
            foreach($c1 as $key => $val)
            {
              echo $c1[$key]."¤òŽÁŽªŽ¯Ž¸¤·¤Þ¤·¤¿¤Í&#63911;&#63911;<br>";
            }
          }
        ?><br>
        <font color="#FF0000">¢£Ž¾ŽÚŽ¸ŽÄŽÎŽÞŽ¯Ž¸Ž½</font><br>
        <?php echo $_POST["s1"]; ?><br>
        <font color="#FF0000">¢£Á÷¿®ŽÎŽÞŽÀŽÝ</font><br>
        <?php echo $_POST["submit"]; ?>
    <br>
    <br>
    &#63879;<a href="3.php" accesskey="1">Ìá¤ë</a><br>
    &#63888;<a href="1.php" accesskey="0">ŽÄŽ¯ŽÌŽß¤ØÌá¤ë</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
