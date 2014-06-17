<?

# This file is part of Chat Informer project
# Copyright (C) 2011 Alexey Novikov, Ilya Golovenko

error_reporting(0);
set_time_limit(30);

include('config.php');
include('../mysql.cfg.php');

$ENCRYPT = false;

if(mysql_pconnect($db_host, $db_login, $db_pass) && mysql_select_db($db_name)) {
    $query = "SELECT COUNT(id) 
                FROM informer";

    $q = mysql_query($query);

    if(mysql_num_rows($q) > 0) {
        $r = mysql_fetch_array($q);
        $total_count = $r[0];
    }

    $time = time() - 7200;

    $query = "SELECT COUNT(id)
                FROM informer
               WHERE ltime > $time";

    $q = mysql_query($query);

    if(mysql_num_rows($q) > 0) {
        $r = mysql_fetch_array($q);
        $active_count = $r[0];
    }
}

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>СПБ ЧАТ ИНФОРМЕР</title>
<STYLE type="text/css">
body, td {
	font-size: 10pt;
	font-family: Tahoma, Arial;
	font-color: Black;
	margin: 0px;
}
a {
	color: #BB8855;
	text-decoration: underline;
}
a:hover {
	color: Red;
	text-decoration: none;
}
h1 {
	color: #BB8855;
	font-size: 14pt;
	margin-left: -10px;
}
.al {
	width: 10px;
	height: 10px;
	border: 0;
	vertical-align: middle;
	margin-right: 2px;
}
</STYLE>
</head>
<body bgcolor=#FFFFFF background="img/back.gif">
<table border=0 cellpadding=2 cellspacing=0 width=100%>
<tr>
<td align=left height=74 bgcolor=#CC9966><a href="/"><img src="img/logo.gif" border=0 width=549 height=74 alt="СПБ ЧАТ ИНФОРМЕР"></a></td>
</tr>
<tr>
<td align=left>
<table border=0 cellpadding=10>
<tr>
<td align=left valign=top style="text-align: justify;">
<h1 id="about">СПБ ЧАТ Информер</h1>
Программа "СПБ ЧАТ Информер" создана специально для активных посетителей <a href="http://spchat.ru/" target=_blank>СПБ ЧАТ</a>а, которые всегда хотят быть в курсе последних событий нашего чата.
С помощью программы можно следить за последними темами нашего форума, в режиме реального времени видеть список посетителей чата, просматривать анкеты зарегистрированных пользователей, читать новости чата,
настраивать уведомления о приходе в чат определеных людей, смотреть последние добавленные в фотоальбом чата фотографии и многое другое. Возможности программы все время совершенствуются и добавляются новые.

<h1 id="view">Вид программы, настройки</h1>
<img src="img/menu.png" width=194 height=291 border=0 alt="Меню" style="float: left; margin: 5px;">
<div style="text-align: justify;">
В главном окне программы можно видеть посетителей в чате, их пол, информацию об общем количестве людей в чате. Двойной клик мыши на строке с ником открывает <a href="img/profile.png" target=_blank><img src="img/globe.gif" class="al">окно с анкетой пользователя</a>. Также можно воспользоваться <a href="img/context.png" target=_blank><img src="img/globe.gif" class="al">контекстным меню</a> в главном окне программы для сортировки ников и дополнительных действий над выбранным элементом. Любому посетителю, находящемуся в данный момент в чате можно <a href="img/message.png" target=_blank><img src="img/globe.gif" class="al">отправить сообщение</a> прямо из информера! На отправку сообщений действуют ограничения, подробнее о них можно узнать в форуме.
В анкете пользователя отображается регистрационная информация посетителя, его фотография (если она есть в фотоальбоме), при клике по фотографии происходит переход в фотоальбом на выбранную фотографию.
Разные иконки у ников в главном окне символизируют разные типы посетителей.
При нажатии на крестик главного окна, оно сворачивается в трей (системный лоток, там где часы), после чего его можно снова открыть нажатием на иконку программы.
Также к этой иконке привязано контекстное меню, среди его команд быстрый переход в чат, информация о программе, настройка будильника чата, показ окошка событий, <a href="img/news.gif" target=_blank><img src="img/globe.gif" class="al">новости чата</a>, <a href="img/birthdays.png" target=_blank><img src="img/globe.gif" class="al">дни рождения в чате</a>, события форума, ссылки, <a href="img/photoalbum.png" target=_blank><img src="img/globe.gif" class="al">фотоальбом</a>, <a href="img/info.png" target=_blank><img src="img/globe.gif" class="al">информация о произвольном нике</a>, настройки программы.
Зеленый цвет иконки в системном лотке сигнализирует о наличии соединения с сервером информера. В <a href="img/alarm.png" target=_blank><img src="img/globe.gif" class="al">окне будильника</a> вы можете настроить события, возникающие при входе (выходе) в чат конкретных пользователей (ников).
Все события информера (сообщения о новых версиях, события будильника, события форума и др.) показываются во <a href="img/forum.png" target=_blank><img src="img/globe.gif" class="al">всплывающем окне событий</a>. Если события запрещены для показа, иконка программы будет мигать при получении нового события, клик по мигающей иконке откроет окно событий. Двойной клик на событии означает его открытие (например для событий форума - открытие соответствующего сообщения в форуме).
Все действия программы настраиваются в <a href="img/settings.gif" target=_blank><img src="img/globe.gif" class="al">окне настроек</a>. Там же можно изменить и <a href="img/proxy.gif" target=_blank><img src="img/globe.gif" class="al">настройки подключения</a>. Для большего удобства можно назначить глобальные горячие клавиши для разных действий программы.
<br>
В некоторых версиях программы (тестовых и начальных) некоторые возможности недоступны, следите за обновлениями!
</div>
<h1 id="download">Скачать программу</h1>
Последняя стабильная версия программы: <b><?=$VERSION['major'];?>.<?=$VERSION['minor'];?>.<?=$VERSION['build'];?></b> (<a href="history.txt" target=_blank>История версий</a>)<br>
Скачать программу: <a href="download/informer_setup_<?=$VERSION['major'];?>.<?=$VERSION['minor'];?>.<?=$VERSION['build'];?>.exe"><b><?=sprintf("%d", filesize("download/informer_setup_$VERSION['major'].$VERSION['minor'].$VERSION['build'].exe")/1024);?> кб</b></a><br>

После скачивания архива программы запустите инсталлятор и следуйте его инструкциям.

<h1 id="stats">Статистика</h1>
Всего рабочих программ: <b><?=$total_count;?></b><br>
Сейчас активных: <b><?=$active_count;?></b>

<h1 id="info">Предложения и замечания</h1>
Все свои предложения, пожелания и замечания, по улучшению программы, сообщения об ошибках можно оставлять в <a href="http://forum.spchat.ru/viewtopic.php?t=3542" target=_blank>этой теме в форуме</a>, либо отправлять непосредственно <a href="mailto: admin@spchat.ru?Subject=Informer">администрации проекта</a>.
</td>
<td align=left valign=top width=200>
<img src="img/main.png" width=180 height=740 border=0 alt="Главное окно программы">
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td align=center bgcolor=#CC9966 style="color:white">
2005, 2006, 2011 &copy; Проект СПБ ЧАТа
</td>
</tr>
<td align=center>
<img src="http://counter.rambler.ru/top100.cnt?501762" alt="" width=1 height=1 border=0>
<script language="JavaScript"><!--
d=document;a='';a+=';r='+escape(d.referrer)
js=10//--></script><script language="JavaScript1.1"><!--
a+=';j='+navigator.javaEnabled()
js=11//--></script><script language="JavaScript1.2"><!--
s=screen;a+=';s='+s.width+'*'+s.height
a+=';d='+(s.colorDepth?s.colorDepth:s.pixelDepth)
js=12//--></script><script language="JavaScript1.3"><!--
js=13//--></script><script language="JavaScript"><!--
d.write('<img src="http://top.list.ru/counter'+
'?id=603566;js='+js+a+';rand='+Math.random()+
'" height=1 width=1>')
if(js>11)d.write('<'+'!-- ')//--></script><noscript><img
src="http://top.list.ru/counter?js=na;id=603566"
height=1 width=1 alt=""></noscript><script language="JavaScript"><!--
if(js>11)d.write('--'+'>')//--></script>
<script language="JavaScript"><!--
document.write('<img src="http://counter.yadro.ru/hit?r'+
escape(document.referrer)+((typeof(screen)=='undefined')?'':
';s'+screen.width+'*'+screen.height+'*'+(screen.colorDepth?
screen.colorDepth:screen.pixelDepth))+';'+Math.random()+
'" width=1 height=1 alt="">')//--></script>
<a href="http://www.liveinternet.ru/click" target=liveinternet><img
src="http://counter.yadro.ru/logo?12.3" border=0
alt="liveinternet.ru: показано число хитов за 24 часа, посетителей за 24 часа и за сегодня"
width=88 height=31></a>
<a target=_blank href="http://top.mail.ru/jump?from=603566"><img
src="http://top.list.ru/counter?id=603566;t=49;l=1"
border=0 height=31 width=88
alt="Рейтинг@Mail.ru"></a>
<script language=JavaScript>
<!--
d=document;server='http://cnt.spbland.ru';
rn=Math.round(Math.random()*1e6);
d.write('<img alt=\'Стартовая страница ' +
'петербургского интернета\' ' +
'src=\''+server+'/?c=392&ref='+escape(d.referrer)+
'&rand='+rn+'\' width=88 height=31 border=0>');
// -->
</script>
<noscript>
<img src="http://cnt.spbland.ru/?c=392" width="88" height="31" border="0" alt="Стартовая страница петербургского интернета">
</noscript>
</td>
</tr>
</table>
</body>
</html>