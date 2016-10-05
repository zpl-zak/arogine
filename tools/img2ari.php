<?php
$img = $argv[1];
$ext = explode(".", $img)[1];
if($ext == "png" || "PNG")
    $im = imagecreatefrompng($img);
elseif($ext == "jpg" || "JPG" || "jpeg" || "JPEG")
    $im = imagecreatefromjpeg($img);
else
    exit;

$size = getimagesize($img);
$width = $size[0];
$height = $size[1];

$handle = fopen($img . ".ari", "wb");
fwrite($handle, pack("I",$width));
fwrite($handle, pack("I",$height));

for ($x = 0; $x < $width; $x++) {
    for ($y = 0; $y < $height; $y++) {
        $rgb = imagecolorat($im, $x, $y);
        $r = ($rgb >> 16) & 0xFF;
        $g = ($rgb >> 8) & 0xFF;
        $b = $rgb & 0xFF;
        fwrite($handle, pack("f", $r / 256));
        fwrite($handle, pack("f", $g / 256));
        fwrite($handle, pack("f", $b / 256));
    }
}
fclose($handle);