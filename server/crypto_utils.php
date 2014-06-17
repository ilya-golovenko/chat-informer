<?

class crypto {
    public static function encrypt($key, $buffer) {
        if($buffer !== null && strlen($buffer) > 0) {
            $td = mcrypt_module_open(MCRYPT_RIJNDAEL_256, '', MCRYPT_MODE_ECB, '');

            $block_size = mcrypt_enc_get_block_size($td);
            $iv_size = mcrypt_enc_get_iv_size($td);

            $iv = str_repeat("\x00", $iv_size);
            mcrypt_generic_init($td, $key, $iv);

            $buffer = add_pkcs7_padding($buffer, $block_size);
            $buffer = mcrypt_generic($td, $buffer);

            mcrypt_generic_deinit($td);
            mcrypt_module_close($td);
        }

        return $buffer;
    }

    public static function decrypt($key, $buffer) {
        if($buffer !== null && strlen($buffer) > 0) {
            $td = mcrypt_module_open(MCRYPT_RIJNDAEL_256, '', MCRYPT_MODE_ECB, '');

            $block_size = mcrypt_enc_get_block_size($td);
            $iv_size = mcrypt_enc_get_iv_size($td);

            $iv = str_repeat(char(0), $iv_size);
            mcrypt_generic_init($td, $key, $iv);

            $buffer = mdecrypt_generic($td, $buffer);
            $buffer = remove_pkcs7_padding($buffer);

            mcrypt_generic_deinit($td);
            mcrypt_module_close($td);
        }

        return $buffer;
    }

    private static function calculate_pkcs7_padding($buffer, $block_size)
    {
        $padding = strlen($buffer) % $block_size;
        return ($padding > 0 ? $padding : $block_size);
    }

    private static function add_pkcs7_padding($buffer, $block_size)
    {
        $padding = calculate_pkcs7_padding($buffer, $block_size);
        return str_pad($buffer, strlen($buffer) + $padding, chr($padding));
    }

    private static function remove_pkcs7_padding($buffer)
    {
        $padding = ord($buffer[strlen($buffer) - 1]);
        return substr($buffer, 0, strlen($buffer) - $padding);
    }
}

?>
