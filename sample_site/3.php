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
      <form name="a" method="post" action="3-1.php">
        �Î����ĎΎގ�����(istyle=1)<br>
        <input type="text" name="i1" maxlength="10" size="10" istyle="1"><br>
        �Î����ĎΎގ�����(istyle=2)<br>
        <input type="text" name="i2" maxlength="10" size="10" istyle="2"><br>
        �Î����ĎΎގ�����(istyle=3)<br>
        <input type="text" name="i3" maxlength="10" size="10" istyle="3"><br>
        �Î����ĎΎގ�����(istyle=4)<br>
        <input type="text" name="i4" maxlength="10" size="10" istyle="4"><br>
        �Î����ĎΎގ�����(istyle�ʤ�)<br>
        <input type="text" name="i5" maxlength="10" size="10"><br>
        �ʎߎ��܎��Ď�<br>
        <input type="password" name="pass" maxlength="10" size="10"><br>
        �׎��ގ��Ύގ���<br>
        <input type="radio" name="r1" value="1">1������<br>
        <input type="radio" name="r1" value="2">2������<br>
        <input type="radio" name="r1" value="3">3������<br>
        ���������Ύގ�����<br>
        <input type="checkbox" name="c1[0]" value="1">1������<br>
        <input type="checkbox" name="c1[1]" value="2">2������<br>
        <input type="checkbox" name="c1[2]" value="3">3������<br>
        ���ڎ��ĎΎގ�����<br>
        <select name="s1">
          <option value=" "> </option>
          <option value="1">1������</option>
          <option value="2">2������</option>
          <option value="3">3������</option>
        </select><br>
        <div align="right"><input type="submit" name="submit" value="  ����1  "></div>
        <input type="submit" name="submit" value="  ����2  ">
      </form>
    <br>
    <br>
    ���<a href="1.php" accesskey="0">�Ď��̎ߤ����</a><br>
    <hr>
    Copyright(C) 2005 Atsushi Konno <br>
    All rights reserved.
  </body>
</html>
