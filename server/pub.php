<?

# This file is part of Chat Informer project
# Copyright (C) 2011 Alexey Novikov, Ilya Golovenko

error_reporting(0);
set_time_limit(10);

include('config.php');
include('errors.php');
include('misc_utils.php');
include('string_utils.php');

//$DEBUG = true;

if($DEBUG)
    $ENCRYPT = false;

if($DEBUG == false) {
    $user_agent = $_SERVER['HTTP_USER_AGENT'];
    $informer_id = $_SERVER['HTTP_X_CHAT_INFORMER_ID'];

    if($user_agent != $UAGENT || $informer_id != $GUID) {
        header('HTTP/1.0 403 Forbidden');
        exit;
    }
}

$request = @file_get_contents('php://input');
$request = crypto::decrypt($CRYPTO_KEY, $request);
$request = json_decode($request);

if($request === null) {
    header('HTTP/1.0 403 Forbidden');
    exit;
}

chdir('..');

define('NO_GZIP', 1);
define('ONLY_FUNCT', 1);

include('__dima.php');
include('__other.php');

# Object in JSON format to be returned to the client
$response = new StdClass();

if(get_load_average() > $MAXLOADAVG) {
    $response->server_status = false;
    add_refresh_time(&$response, 'overload');
} else {
    $response->server_status = true;
    process_client_request($request, &$response);
}

echo json_encode($response, JSON_UNESCAPED_SLASHES | JSON_UNESCAPED_UNICODE);

function process_client_request($request, $response) {
    if(isset($request->auth) && isset($request->auth_data)) {
        $response->auth_result = authorize_user($request->auth, $request->auth_data);
        add_refresh_time(&$response, 'auth');

        if(!$response->auth_result->success) {
            return;
        }
    }

    if(isset($request->message) && isset($request->auth_data)) {
        $response->send_result = send_message($request->message, $request->auth_data);
        return;
    }

    if(isset($request->user)) {
        $response->user_info = load_user_info($request->user);
        return;
    }

    if(isset($request->info)) {
        $response->server_info = load_server_info();
    }

    if(isset($request->userlist)) {
        $response->userlist = load_userlist();
        add_refresh_time(&$response, 'userlist');
    }

    if(isset($request->news)) {
        $response->news = load_news();
        add_refresh_time(&$response, 'news');
    }

    if(isset($request->links)) {
        $response->links = load_links();
    }

    if(isset($request->forum)) {
        $response->forum = load_forum();
        add_refresh_time(&$response, 'forum');
    }

    if(isset($request->photoalbum)) {
        $response->photoalbum = load_photoalbum();
        add_refresh_time(&$response, 'photoalbum');
    }

    if(isset($request->birthdays)) {
        $response->birthdays = load_birthdays();
        add_refresh_time(&$response, 'birthdays');
    }

    if(isset($request->icons)) {
        $response->icons = load_icons();
    }
}

function add_refresh_time($data, $name) {
    global $TIME;

    if(!isset($data->refresh)) {
        $data->refresh = new StdClass();
    }

    $data->refresh->$name = $TIME[$name];
}

function check_user_credentials($nickname, $password) {
    if(dbload(strtolower($nickname), $info) == true) {
        if(checkpass($password, md5($info['pass'])) == true) {
            return (empty($info[11]['active_key']) && @$info[11]['good'] != 3);
        }
    }

    return false;
}

function load_server_info() {
    global $UAGENT, $CHATNAME, $CHATURL, $VERSION;

    return array(
        'name'    => $UAGENT,
        'chat'    => $CHATNAME,
        'url'     => $CHATURL,
        'version' => $VERSION
    );
}

function authorize_user($auth, $auth_data) {
    $auth_result = new StdClass();

    if(isset($auth_data->credentials)) {
        $credentials = $auth_data->credentials;
        $credentials = base64_decode($credentials);
        $credentials = explode(':', $credentials);

        $nickname = $credentials[0]; // nickname
        $password = $credentials[1]; // password hash

        if(!check_user_credentials($nickname, $password)) {
            $auth_result->bad_credentials = true;

            unset($nickname);
            unset($password);
        }
    }

    $id = $auth_data->instance_id;
    $version = $auth_data->version;

    if(!empty($id)) {
        include('../mysql.cfg.php');

        $ip = $_SERVER['REMOTE_ADDR'];
        $country = get_country_code($ip);

        if(mysql_connect($db_host, $db_login, $db_pass) && mysql_select_db($db_name)){
            $id = mysql_real_escape_string($id);
            $version = mysql_real_escape_string($version);
            $nickname = mysql_real_escape_string($nickname);

            $query = "SELECT id, ban
                        FROM informer
                       WHERE guid = '$id'
                       LIMIT 1";

            $q = mysql_query($query);

            if(mysql_num_rows($q) > 0) {
                $r = mysql_fetch_assoc($q);

                if($r['ban'] == 0) {
                    $id = mysql_real_escape_string($r['id']);

                    $query = "UPDATE informer
                                 SET version = '$version',
                                     runs = runs + 1,
                                     ltime = UNIX_TIMESTAMP(),
                                     unick = '$nickname',
                                     inet_type = 'unknown',
                                     ip = '$ip',
                                     country = '$country'
                               WHERE id = '$id'";

                    mysql_query($query);

                    if(mysql_affected_rows()) {
                        $auth_result->success = true;
                        $auth_result->id = $id;
                    } else {
                        $auth_result->success = false;
                        $auth_result->error = ERROR_DB_QUERY;
                    }
                } else {
                    $auth_result->success = false;
                    $auth_result->error = ERROR_ID_BANNED;
                }
            } else {
                $query = "INSERT INTO informer
                                  SET guid = '$id',
                                      version = '$version',
                                      ltime = UNIX_TIMESTAMP(),
                                      runs = 1,
                                      unick = '$nickname',
                                      inet_type = 'unknown',
                                      ip = '$ip',
                                      country = '$country'";

                mysql_query($query);

                if(mysql_affected_rows()) {
                    $auth_result->success = true;
                    $auth_result->id = mysql_insert_id();
                } else {
                    $auth_result->success = false;
                    $auth_result->error = ERROR_DB_QUERY;
                }
            }
        } else {
            $auth_result->success = false;
            $auth_result->error = ERROR_DB_CONNECT;
        }
    } else {
        $auth_result->success = false;
        $auth_result->error = ERROR_NEED_ID;
    }

    return $auth_result;
}

function create_message($nickname, $text) {
    global $CHATPATH;

    $message = @file_get_contents($CHATPATH.'informer/message.html');
    $message = str_replace('<!-- %TEXT% -->', msg2html($text), $message);
    $message = str_replace('<!-- %NICKNAME% -->', htmlspecialchars($nickname), $message);

    return $message;
}

function send_message($message, $auth_data) {
    global $user;

    $send_result = new StdClass();

    $credentials = $auth_data->credentials;
    $credentials = base64_decode($credentials);
    $credentials = explode(':', $credentials);

    $nickname = $credentials[0]; // nickname
    $password = $credentials[1]; // password hash

    $recipient = convert_to_cp1251($message->nickname);
    $text = convert_to_cp1251($message->text);

    $id = $auth_data->instance_id;
    $version = $auth_data->version;

    if(check_user_credentials($nickname, $password)) {
        include('../mysql.cfg.php');
        include('src/msg.php');
        include('swf.php');

        if(isset($user[$recipient])) {
            if($text == do_chat($text, 'silent')) {
                if(dbload(strtolower($recipient), $info) == true && !empty($info[11]['informer'])) {
                    if(mysql_connect($db_host, $db_login, $db_pass) && mysql_select_db($db_name)) {
                        if(!@array_search(strtolower($nickname), $info[11]['ignore'])) {
                            if(time() - 10 > $r['ltime']) {
                                m_private($recipient, create_message($nickname, $text));

                                $ip = $_SERVER['REMOTE_ADDR'];

                                $id = mysql_real_escape_string($id);
                                $nickname = mysql_real_escape_string($nickname);
                                $recipient = mysql_real_escape_string($recipient);
                                $text = mysql_real_escape_string(htmlspecialchars($text));

                                $query = "INSERT INTO chat_log
                                                  SET dt = NOW(),
                                                      mtype = 'inf',
                                                      fromnick = '$nickname',
                                                      informer = '$id',
                                                      tonick = '$recipient',
                                                      ip = '$ip',
                                                      text = '$recipient: $text'";

                                mysql_query($query);

                                $query = "UPDATE informer
                                             SET ltime = UNIX_TIMESTAMP(),
                                                 version = '$version',
                                                 unick = '$nickname'
                                           WHERE id = '$id'";

                                mysql_query($query);

                                $send_result->success = true;
                            }else {
                                $send_result->success = false;
                                $send_result->error = ERROR_SHOULD_WAIT;
                            }
                        }else {
                            $send_result->success = false;
                            $send_result->error = ERROR_IGNORED;
                        }
                    } else {
                        $send_result->success = false;
                        $send_result->error = ERROR_DB_CONNECT;
                    }
                } else {
                    $send_result->success = false;
                    $send_result->error = ERROR_SEND_DENIED;
                }
            } else {
                $send_result->success = false;
                $send_result->error = ERROR_GENERIC;
            }
        } else {
            $send_result->success = false;
            $send_result->error = ERROR_OFFLINE;
        }
    } else {
        $send_result->success = false;
        $send_result->error = ERROR_BAD_CREDENTIALS;
    }

    return $send_result;
}

function load_photoalbum() {
    global $PHOTOPATH, $PHOTOMAX;

    $photoalbum = new StdClass();
    $photoalbum->photos = array();

    include($PHOTOPATH.'includes/dbconfig.php');

    if(mysql_connect($mysql_host, $mysql_login, $mysql_passwd) && mysql_select_db($mysql_dbname)) {
        mysql_set_charset('utf8');

        $table_list = PREFIX.'list';

        $query = "SELECT id, element_name, element_path, user_nick
                    FROM $table_list
                   WHERE element_status = 1
                   ORDER BY id DESC
                   LIMIT $PHOTOMAX";

        $q = mysql_query($query);

        while($r = mysql_fetch_assoc($q)) {
            $photo = array(
                'id'    => strval($r['id']),
                'thumb' => $r['element_path'],
                'descr' => $r['element_name'],
            );

            if($r['user_nick'] !== "") {
                $photo['nickname'] = $r['user_nick'];
            }

            $photoalbum->photos[] = $photo;
        }

        mysql_free_result($q);
    }

    return $photoalbum;
}

function load_birthdays() {
    $data = @file_get_contents('../informer_birthday.txt');
    $nicknames = explode(' ', convert_to_utf8($data));

    return array(
        'today'     => date('d.m.Y'),
        'nicknames' => $nicknames
    );
}

function load_icons() {
    $icons = array();

    include('../mysql.cfg.php');

    if(mysql_connect($db_host, $db_login, $db_pass) && mysql_select_db($db_name)) {
        mysql_set_charset('utf8');

        $query = "SELECT nick, icon
                    FROM informer_users
                   WHERE active = 1";

        $q = mysql_query($query);

        while($r = mysql_fetch_assoc($q)) {
            $icons[] = array(
                'nickname' => $r['nick'],
                'filename' => $r['icon']
            );
        }

        mysql_free_result($q);
    }

    return $icons;
}

function load_news() {
    global $conf;

    return array(
        'admin' => convert_to_utf8($conf['topic']),
        'moder' => convert_to_utf8(@file_get_contents('src/mnews.html'))
    );
}

function load_links() {
    global $CHATLINKS;

    $links = array();

    foreach($CHATLINKS as $key => $value) {
        $link = array(
            'url'   => $key,
            'name'  => $value[0],
            'descr' => $value[1]
        );

        $links[] = $link;
    }

    return $links;
}

function load_userlist() {
    global $user;

    $userlist = array();

    foreach($user as $key => $value) {
        $auth = intval($value['auth']);
        $sex = intval($value['sex']);

        $userlist[] = array(
            convert_to_utf8($key),
            $auth > 0 ? $sex : -1
        );
    }

    return $userlist;
}

function load_forum() {
    global $FORUMPATH, $FORUMURL, $TIME;

    $forum = new StdClass();

    define('IN_PHPBB', true);
    include($FORUMPATH.'config.php');
    include($FORUMPATH.'includes/constants.php');

    if(mysql_connect($dbhost, $dbuser, $dbpasswd) && mysql_select_db($dbname)) {
        mysql_set_charset('utf8');

        $table_topics = TOPICS_TABLE;
        $table_posts = POSTS_TABLE;
        $table_users = USERS_TABLE;

        $start_time = time() - $TIME['forum'] - 10;

        $query = "SELECT posts.post_id, posts.post_time, posts.post_username,
                         topics.topic_id, topics.topic_title,
                         users.user_id, users.username
                    FROM $table_posts posts
                    JOIN $table_topics topics ON topics.topic_id = posts.topic_id
                    JOIN $table_users users ON users.user_id = posts.poster_id
                   WHERE posts.post_time > $start_time
                     AND posts.post_approved = 1
                ORDER BY topics.topic_id, posts.post_time";

        $q = mysql_query($query);

        if(mysql_num_rows($q) > 0) {
            $topics = array();

            while($r = mysql_fetch_assoc($q)) {
                if($r['user_id'] != ANONYMOUS) {
                    $post_author = $r['username'];
                } elseif(!empty($r['post_username'])) {
                    $post_author = $r['post_username'];
                } else {
                    $post_author = 'Гость';
                }

                $post = array(
                    'id'     => $r['post_id'],
                    'author' => $post_author,
                    'time'   => intval($r['post_time'])
                );

                $is_new_topic = !isset($topic)
                    || $topic[0] != $r['topic_id'];

                if($is_new_topic) {
                    if(isset($topic)) {
                        $topic[] = $posts;
                        $topics[] = $topic;
                    }

                    $topic = array(
                        'id'    => $r['topic_id'],
                        'title' => $r['topic_title']
                    );

                    $posts = array();
                }

                $posts[] = $post;
            }

            $topic[] = $posts;
            $topics[] = $topic;

            $forum->topics = $topics;
        }

        mysql_free_result($q);
    }

    return $forum;
}

function load_user_info($user) {
    global $PHOTOPATH, $USERINFOFIELDS;

    $user_info = new StdClass();

    $nickname = convert_to_cp1251($user->nickname);
    $nickname = strtolower($nickname);

    if(dbload($nickname, $info) == true) {
        $obj = unserialize(myfile('src/form_loginform.txt'));

        foreach($obj as $key => $value) {
            if(array_key_exists($key, $USERINFOFIELDS)) {
                $field = $USERINFOFIELDS[$key];

                if(isset( $info[$key])) {
                    $value = $info[$key];

                    if(is_string($value)) {
                        $value = convert_to_utf8($value);

                        if($value !== "") {
                            $user_info->$field = $value;
                        }
                    } else {
                        $user_info->$field = $value;
                    }
                }
            }
        }

        include($PHOTOPATH.'includes/dbconfig.php');

        if(mysql_connect($mysql_host, $mysql_login, $mysql_passwd) && mysql_select_db($mysql_dbname)) {
            $table_list = PREFIX.'list';

            $nickname = mysql_real_escape_string($nickname);

            $query = "SELECT id, element_path
                        FROM $table_list
                       WHERE element_category IN(1, 2, 4, 6)
                         AND user_nick = '$nickname'
                         AND element_status = 1
                       ORDER BY RAND()
                       LIMIT 5";

            $q = mysql_query($query);

            if(mysql_num_rows($q) > 0) {
                $photos = array();

                while($r = mysql_fetch_assoc($q)) {
                    $photos[] = array(
                        'id'    => strval($r['id']),
                        'thumb' => r['element_path']
                    );
                }

                $user_info->photos = $photos;
            }

            mysql_free_result($q);
        }

        $user_info->reg_time = $info[11]['reg_time'];
        $user_info->last_join = $info[11]['last_join'];
    } else {
        $user_info->not_found = true;
    }

    return $user_info;
}

?>
