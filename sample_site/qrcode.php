<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<?php
  $uuid       = $_GET["uuid"];
  $contractId = $_GET["contractId"];
  $url = "http://".$contractId.".google.co.jp/?q=".$uuid."+".$contractId;

header("Content-Type:text/xml; charset=EUCJP-WIN");
  
echo "<?xml version=\"1.0\" ?>\n";
echo "\n";
echo "<qrcode>\n";
echo "  <version>6</version>\n";
echo "  <level>L</level>\n";
echo "  <mode>8bit</mode>\n";
echo "  <size>4</size>\n";
echo "  <data>".$url."</data>\n";
echo "</qrcode>\n";
?>
