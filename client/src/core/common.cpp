//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2012, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>


namespace chat
{

std::string const informer_name           = "SpChat.Ru Chat Informer";
version const informer_version            = version(2, 0, 0);

std::string const informer_hostname       = "informer.spchat.ru";
std::string const informer_pub_path       = "/pub2.php";
std::string const informer_icon_path      = "/ico/";

std::string const news_template_path      = "/news.html";

std::string const informer_setup_path     = "/download/";
std::string const informer_setup_file     = "informer_setup_{0}.{1}.{2}.exe";
std::string const informer_setup_params   = "/S /D=\"{0}\"";

missio::format::string const user_info_uri("http://spchat.ru/login.php?c=info&ni={0}");
missio::format::string const forum_post_uri("http://forum.spchat.ru/viewtopic.php?p={0}#p{0}");

missio::format::string const photoalbum_thumb_uri("http://photo.spchat.ru/img/pre/{0}");
missio::format::string const photoalbum_photo_uri("http://photo.spchat.ru/index.php?a=view&n={0}");

missio::format::string const email_uri("mailto:{0}");
missio::format::string const vkontakte_uri("http://vk.com/id{0}");
missio::format::string const icq_uri("http://www.icq.com/people/{0}");

boost::uuids::uuid const informer_id =
{
    0xBE, 0x48, 0xDF, 0x66, 0x9B, 0xF9, 0x56, 0x6B,
    0xA3, 0xA4, 0xFF, 0x7C, 0xD8, 0x72, 0xAB, 0x3A
};

boost::uuids::uuid const crypto_key =
{
    0xBF, 0xAA, 0x5C, 0x29, 0x72, 0xE9, 0x47, 0x8E,
    0x9B, 0xE9, 0x37, 0xC4, 0x56, 0x1A, 0x70, 0x5C
};

}   // namespace chat
