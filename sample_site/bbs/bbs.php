<html>
<head>
<title>簡易掲示板</title>
</head>
<body>
<h1>
簡易掲示板</h1>
<form action="bbs.php" method="POST">
名前<input type="text"  maxlength="30"  size="15"  name="name"  ><br>
件名<input type="text"  maxlength="120" size="60"  name="subject"  ><br>
記事<textarea  name="message" cols="60" rows="5">
</textarea>
<br>

<input type="submit"  name="送信"  >
<input type="reset"  name="リセット"  ><br>
</form>
<?php
if ($_POST["name"] == "" && $_POST["subject"] == "" && $_POST["message"] == "") {
  echo readfile("./bbs.data");
}
else {
  $data = file_get_contents("./bbs.data");
  $fp = fopen("./bbs.data", "wb");
  $subject = $_POST["subject"];
  $message = $_POST["message"];
  $name = $_POST["name"];
  $time = date("Y-m-d H:i:s");
  $num = count(split("\<dt\>", $data));
  $last_message = "<dt>$num $subject ($name)  $time</dt>\n";
  $last_message .="<dd>$message</dd>\n";
  $data = $last_message . $data;

  fwrite($fp, $data);
  fclose($fp);

  echo readfile("./bbs.data");
}
?>
</body>

</html>
