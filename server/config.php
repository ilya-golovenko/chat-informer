<?

# ===== CONFIG OPTIONS FOR THIS SCRIPT ======

$GUID       = 'BE48DF669BF9566BA3A4FF7CD872AB3A';
$CRYPTO_KEY = pack('H*', 'BFAA5C2972E9478E9BE937C4561A705C');
$ENCRYPT    = true;
$DEBUG      = false;
$UAGENT     = 'SpChat.Ru Chat Informer';
$VERSION    = array('major' => 2, 'minor' => 0, 'build' => 157);
$CHATNAME   = 'СПБ ЧАТ';
$CHATURL    = 'http://spchat.ru/';
$CHATPATH   = '/home/spchat.ru/www/';
$FORUMPATH  = '/home/forum.spchat.ru/public_html/';
$PHOTOPATH  = '/home/photo.spchat.ru/public_html/';
$PHOTOMAX   = 30;
$MAXLOADAVG = 17;

$bt = mktime(0, 0, 0, date('m'), date('d') + 1, date('Y')) + 3600 - time();

if($bt > 86400)
    $bt = 86400 - $bt;

$TIME = array(
    'auth'          => 7200,
    'news'          => 3600,
    'forum'         => 180,
    'birthdays'     => $bt + mt_rand(100, 600),
    'photoalbum'    => 3600,
    'photocomments' => 720,
    'overload'      => mt_rand(60, 300)
);

$CHATLINKS = array(
    'http://spchat.ru/'          => array('СПБ ЧАТ ПОРТАЛ', 'Первый молодёжный Питерский портал'),
    'http://forum.spchat.ru/'    => array('Форум СПБ ЧАТа', 'Официальный форум СПБ ЧАТа - Разговоры обо всём!'),
    'http://blog.spchat.ru/'     => array('СПБ БЛОГ', 'СПБ БЛОГ - лента событий проекта'),
    'http://photo.spchat.ru/'    => array('Фотоальбом СПБ ЧАТа', 'Фотоальбом нашего чата, фотографии наших посетителей и встреч чата'),
    'http://informer.spchat.ru/' => array('ЧАТ-Информер', 'Страничка программы ЧАТ-Информер'),
    'http://mults.spb.ru/'       => array('МультПортал', 'Мультики - пополняемая коллекция мультфильмов!'),
    'http://chatradio.ru/'       => array('Радио ЧатПортала', 'Лучшее интернет-радио для всех возрастов!'),
    'http://rapidlinks.ru/'      => array('RapidLinks.Ru', 'Поиск файлов на RapidShare')
);

$USERINFOFIELDS = array(
    'nick'          => 'nickname',
    'name'          => 'fullname',
    'year'          => 'birth_year',
    'birth_month'   => 'birth_month',
    'birth_day'     => 'birth_day',
    'pubmail'       => 'email',
    'location'      => 'location',
    'sex'           => 'sex',
    'icq'           => 'icq',
    'vkontakte'     => 'vkontakte',
    'phone'         => 'phone',
    'url'           => 'homepage',
    'eye'           => 'eyes',
    'hair'          => 'hair',
    'height'        => 'height',
    'about'         => 'about',
    'real'          => 'meetings',
    'froma'         => 'knows_from'
);

# ===========================================

function ob_handler($buffer) {
    global $ENCRYPT, $CRYPTO_KEY;

    header('Last-Modified: '.gmdate('r'));
    header('Expires: '.gmdate('r', 0));

    if($ENCRYPT) {
        include_once('crypto_utils.php');
        header('Content-Type: application/x-encrypted');
        return crypto::encrypt($CRYPTO_KEY, $buffer);
    }

    header('Content-Type: application/json; charset=utf-8');
    return $buffer;
};

ob_start('ob_handler');

?>
