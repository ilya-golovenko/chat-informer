//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_chat_user_info_hpp
#define _missio_core_chat_user_info_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/chat_user.hpp>
#include <core/photo_list.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// BOOST headers
#include <boost/shared_ptr.hpp>

// STL headers
#include <string>
#include <ctime>


namespace missio
{

class chat_user_info :
    private boost::noncopyable
{
public:
    typedef boost::shared_ptr<chat_user_info> pointer;

    static pointer create(json::object_cref json_data);
    static bool contains_user_info(json::object_cref json_data);

public:
    explicit chat_user_info(json::object_cref json_data);
    ~chat_user_info();

    std::wstring const& nickname() const;
    std::wstring const& fullname() const;
    std::size_t birth_day() const;
    std::size_t birth_year() const;
    std::wstring const& birth_month() const;
    std::wstring const& location() const;
    std::wstring const& homepage() const;
    std::wstring const& email() const;
    std::wstring const& icq() const;
    std::wstring const& vkontakte() const;
    std::wstring const& phone() const;
    std::wstring const& eyes() const;
    std::wstring const& hair() const;
    std::wstring const& height() const;
    std::wstring const& meetings() const;
    std::wstring const& knows_from() const;
    std::wstring const& about() const;
    chat_user::sex_type sex() const;
    std::time_t reg_time() const;
    std::time_t last_join() const;

    photo_list const& photos() const;

    std::wstring get_icq_uri() const;
    std::wstring get_email_uri() const;
    std::wstring get_vkontakte_uri() const;
    std::wstring get_user_info_uri() const;

private:
    std::wstring nickname_;
    std::wstring fullname_;
    std::size_t birth_day_;
    std::size_t birth_year_;
    std::wstring birth_month_;
    std::wstring location_;
    std::wstring homepage_;
    std::wstring email_;
    std::wstring icq_;
    std::wstring vkontakte_;
    std::wstring phone_;
    std::wstring eyes_;
    std::wstring hair_;
    std::wstring height_;
    std::wstring meetings_;
    std::wstring knows_from_;
    std::wstring about_;
    chat_user::sex_type sex_;
    std::time_t reg_time_;
    std::time_t last_join_;
    photo_list photos_;
};

}   // namespace missio

#endif  // _missio_core_chat_user_info_hpp
