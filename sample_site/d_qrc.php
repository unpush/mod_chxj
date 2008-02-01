<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<?php
  echo "<?xml encoding=\"EUC-JP\" ?>\n";
  echo "<qrcode>\n";
  echo "<version>9</version>\n";
  echo "<level>H</level>\n";
  echo "<mode>8bit</mode>\n";
  echo "<data>abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz</data>\n";
  echo "</qrcode>\n";
?>
