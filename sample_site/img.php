<?php
header("Cache-Control: no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<?php
  $len = filesize("./img/logo.jpg");
  $fp = fopen("./img/logo.jpg", "rb");
  $data = fread($fp, $len);

  fclose($fp);
  header("Content-Type: image/jpeg");
  header("Content-Length: ".$len);

  echo $data;
?>
