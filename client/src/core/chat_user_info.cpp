//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/exception.hpp>
#include <core/chat_user_info.hpp>

// MISSIO headers
#include <missio/unicode/convert.hpp>
#include <missio/format/format.hpp>


namespace chat
{

chat_user_info::pointer chat_user_info::create(missio::json::object const& json_data)
{
    return std::make_shared<chat_user_info>(json_data);
}

bool chat_user_info::contains_user_info(missio::json::object const& json_data)
{
    return !json_data.contains("not_found") && json_data.contains("nickname");
}

chat_user_info::chat_user_info(missio::json::object const& json_data) :
    sex_(chat_user::unknown),
    reg_time_(0),
    last_join_(0)
{
    if(!contains_user_info(json_data))
        throw exception("cannot create user info");

    nickname_ = json_data["nickname"].get<std::wstring>();
    fullname_ = json_data["fullname"].get<std::wstring>();
    birth_day_ = json_data["birth_day"].get<std::size_t>();
    birth_year_ = json_data["birth_year"].get<std::size_t>();
    birth_month_ = json_data["birth_month"].get<std::wstring>();
    location_ = json_data["location"].get<std::wstring>();
    homepage_ = json_data["homepage"].get<std::wstring>();
    icq_ = json_data["icq"].get<std::wstring>();
    email_ = json_data["email"].get<std::wstring>();
    phone_ = json_data["phone"].get<std::wstring>();
    eyes_ = json_data["eyes"].get<std::wstring>();
    hair_ = json_data["hair"].get<std::wstring>();
    height_ = json_data["height"].get<std::wstring>();
    meetings_ = json_data["meetings"].get<std::wstring>();
    knows_from_ = json_data["knows_from"].get<std::wstring>();
    about_ = json_data["about"].get<std::wstring>();
    reg_time_ = json_data["reg_time"].get<std::time_t>();
    last_join_ = json_data["last_join"].get<std::time_t>();

    photos_.update(json_data);

    if(json_data.contains("sex"))
        sex_ = json_data["sex"].get<chat_user::sex_type>();

    if(json_data.contains("vkontakte"))
        missio::format::write(vkontakte_, json_data["vkontakte"].get<int>());
}

std::wstring const& chat_user_info::nickname() const
{
    return nickname_;
}

std::wstring const& chat_user_info::fullname() const
{
    return fullname_;
}

std::size_t chat_user_info::birth_day() const
{
    return birth_day_;
}

std::size_t chat_user_info::birth_year() const
{
    return birth_year_;
}

std::wstring const& chat_user_info::birth_month() const
{
    return birth_month_;
}

std::wstring const& chat_user_info::location() const
{
    return location_;
}

std::wstring const& chat_user_info::homepage() const
{
    return homepage_;
}

std::wstring const& chat_user_info::email() const
{
    return email_;
}

std::wstring const& chat_user_info::icq() const
{
    return icq_;
}

std::wstring const& chat_user_info::vkontakte() const
{
    return vkontakte_;
}

std::wstring const& chat_user_info::phone() const
{
    return phone_;
}

std::wstring const& chat_user_info::eyes() const
{
    return eyes_;
}

std::wstring const& chat_user_info::hair() const
{
    return hair_;
}

std::wstring const& chat_user_info::height() const
{
    return height_;
}

std::wstring const& chat_user_info::meetings() const
{
    return meetings_;
}

std::wstring const& chat_user_info::knows_from() const
{
    return knows_from_;
}

std::wstring const& chat_user_info::about() const
{
    return about_;
}

chat_user::sex_type chat_user_info::sex() const
{
    return sex_;
}

std::time_t chat_user_info::reg_time() const
{
    return reg_time_;
}

std::time_t chat_user_info::last_join() const
{
    return last_join_;
}

photo_list const&  chat_user_info::photos() const
{
    return photos_;
}

std::wstring chat_user_info::get_icq_uri() const
{
    std::string uri;

    if(!icq_.empty())
        missio::format::print(uri, icq_uri, icq_);

    return missio::unicode::to_wide_string(uri);
}

std::wstring chat_user_info::get_email_uri() const
{
    std::string uri;

    if(!email_.empty())
        missio::format::print(uri, email_uri, email_);

    return missio::unicode::to_wide_string(uri);
}

std::wstring chat_user_info::get_vkontakte_uri() const
{
    std::string uri;

    if(!vkontakte_.empty())
        missio::format::print(uri, vk_uri, vkontakte_);

    return missio::unicode::to_wide_string(uri);
}

std::wstring chat_user_info::get_user_info_uri() const
{
    std::string uri;

    if(!nickname_.empty())
        missio::format::print(uri, user_info_uri, nickname_);

    return missio::unicode::to_wide_string(uri);
}

}   // namespace chat
