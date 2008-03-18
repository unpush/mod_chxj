<html>
  <head>
    <title>ソフトバンクの予約クエリのテスト</title>
  </head>
  <body>
<?php
echo "POSTDATA<br>";
var_dump($_POST);
echo "<hr>";
echo "GETDATA<br>";
var_dump($_GET);
echo "<hr>";
?>
<hr>
<form name="a" method="post" action="1.php">
  <input type="text" name="pid"   value="pid">
  <input type="text" name="sid"   value="sid">
  <input type="text" name="uid"   value="uid">
  <input type="text" name="lid"   value="lid">
  <input type="text" name="gid"   value="gid">
  <input type="text" name="rpid"  value="rpid">
  <input type="text" name="rsid"  value="rsid">
  <input type="text" name="nl"    value="nl">
  <input type="text" name="cl"    value="cl">
  <input type="text" name="ol"    value="ol">
  <input type="text" name="pl"    value="pl">
  <input type="text" name="jskya" value="jskya">
  <input type="text" name="prc"   value="prc">
  <input type="text" name="cnt"   value="cnt">
  <input type="text" name="reg"   value="reg">
  <input type="text" name="vsekey" value="vsekey">
  <input type="text" name="vsernk" value="vsernk">
  <input type="submit">
</form>
<hr>
<form name="a" method="get" action="1.php">
  <input type="text" name="pid"   value="pid">
  <input type="text" name="sid"   value="sid">
  <input type="text" name="uid"   value="uid">
  <input type="text" name="lid"   value="lid">
  <input type="text" name="gid"   value="gid">
  <input type="text" name="rpid"  value="rpid">
  <input type="text" name="rsid"  value="rsid">
  <input type="text" name="nl"    value="nl">
  <input type="text" name="cl"    value="cl">
  <input type="text" name="ol"    value="ol">
  <input type="text" name="pl"    value="pl">
  <input type="text" name="jskya" value="jskya">
  <input type="text" name="prc"   value="prc">
  <input type="text" name="cnt"   value="cnt">
  <input type="text" name="reg"   value="reg">
  <input type="text" name="vsekey" value="vsekey">
  <input type="text" name="vsernk" value="vsernk">
  <input type="submit">
</form>
  </body>
</html>
