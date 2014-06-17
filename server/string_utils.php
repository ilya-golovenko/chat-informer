<?

function remove_spaces($s) {
    return preg_replace("/[\n\r\t\"]+/", '', $s);
}

function normalize_spaces($s) {
    $s = preg_replace("/[\x00-\x1F]+/", ' ', $s);
    return preg_replace("/[ ]{2,}/", ' ', $s);
}

function remove_non_digits($s) {
    return preg_replace("/[^\d]+/", '', $s);
}

function convert_to_utf8($s) {
    return iconv('cp1251', 'utf-8', normalize_spaces($s));
}

function convert_to_cp1251($s) {
    return normalize_spaces(iconv('utf-8', 'cp1251', $s));
}

function get_country_code($ip) {
    require_once('src/geoip/geoip.inc');

    $geoip = geoip_open('/usr/local/share/GeoIP/GeoIP.dat', GEOIP_STANDARD);
    $country = geoip_country_code_by_addr($geoip, $ip);
    geoip_close($geoip);

    return $country;
}

function split_long_word($s) {
    $s = str_replace("\\\"", "\"", $s);
    $s = wordwrap($s, 21, "\x03wbr\x04", 1);
    $return "\x03nobr\x04".$s."\x03/nobr\x04";
}

function msg2html($msg) {
    $msg = preg_replace_callback("/([^ ]{20,})/", 'split_long_word', $msg);

    $msg = htmlspecialchars($msg);
    $msg = str_replace("\x03", '<', $msg);
    $msg = str_replace("\x04", '>', $msg);

    return $msg;
}

?>
