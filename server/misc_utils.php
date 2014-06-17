<?

function get_load_average() {
    $stats = file_get_contents('../daemon/www-server/stats.tmp');

    $string_value = explode('load average: ', $stats);
    $values = explode(', ', $string_value[1]);

    $count = count($values);
    $sum = array_sum($values);

    if($count > 0) {
        return ($sum / $count);
    }

    return 0;
}

?>
