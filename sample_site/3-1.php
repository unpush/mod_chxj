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
    ���ώ̎����ѤΎÎ���1
    </center>
    <hr>
        <font color="#FF0000">���Î����ĎΎގ�����(istyle=1)</font><br>
        <?php echo $_POST["i1"] ?><br>
        <font color="#FF0000">���Î����ĎΎގ�����(istyle=2)</font><br>
        <?php echo $_POST["i2"] ?><br>
        <font color="#FF0000">���Î����ĎΎގ�����(istyle=3)</font><br>
        <?php echo $_POST["i3"] ?><br>
        <font color="#FF0000">���Î����ĎΎގ�����(istyle=4)</font><br>
        <?php echo $_POST["i4"] ?><br>
        <font color="#FF0000">���Î����ĎΎގ�����(istyle=�ʤ�)</font><br>
        <?php echo $_POST["i5"] ?><br>
        <font color="#FF0000">���ʎߎ��܎��Ď�</font><br>
        <?php echo $_POST["pass"]; ?><br>
        <font color="#FF0000">���׎��ގ��Ύގ���</font><br>
        <?php echo $_POST["r1"]; ?><br>
        <font color="#FF0000">�����������Ύގ�����</font><br>
        <?php 
          $c1 = $_POST["c1"]; 
          if ($c1) {
            foreach($c1 as $key => $val)
            {
              echo $c1[$key]."������������ޤ�����&#63911;&#63911;<br>";
            }
          }
        ?><br>
        <font color="#FF0000">�����ڎ��ĎΎގ�����</font><br>
        <?php echo $_POST["s1"]; ?><br>
        <font color="#FF0000">�������Ύގ���</font><br>
        <?php echo $_POST["submit"]; ?>
    <br>
    <br>
    &#63879;<a href="3.php" accesskey="1">���</a><br>
    &#63888;<a href="1.php" accesskey="0">�Ď��̎ߤ����</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
