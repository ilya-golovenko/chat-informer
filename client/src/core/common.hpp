//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2012, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_common_hpp
#define _chat_core_common_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/version.hpp>

// MISSIO headers
#include <missio/format/string.hpp>

// BOOST headers
#include <boost/uuid/uuid.hpp>

// STL headers
#include <string>


namespace chat
{

extern std::string const informer_name;
extern version const informer_version;

extern std::string const informer_hostname;
extern std::string const informer_pub_path;
extern std::string const informer_icon_path;

extern std::string const news_template_path;

extern std::string const informer_setup_path;
extern std::string const informer_setup_file;
extern std::string const informer_setup_params;

extern missio::format::string const user_info_uri;
extern missio::format::string const forum_post_uri;

extern missio::format::string const photoalbum_thumb_uri;
extern missio::format::string const photoalbum_photo_uri;

extern missio::format::string const email_uri;
extern missio::format::string const icq_uri;
extern missio::format::string const vk_uri;

extern boost::uuids::uuid const informer_id;

extern boost::uuids::uuid const crypto_key;

}   // namespace chat

#endif  // _chat_core_common_hpp
