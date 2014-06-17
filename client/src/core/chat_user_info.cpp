//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/exception.hpp>
#include <core/chat_user_info.hpp>

// MISSIO headers
#include <missio/format/format.hpp>
#include <missio/utf8/convert.hpp>

// BOOST headers
#include <boost/make_shared.hpp>


namespace missio
{

chat_user_info::pointer chat_user_info::create(json::object_cref json_data)
{
    return boost::make_shared<chat_user_info>(json_data);
}

bool chat_user_info::contains_user_info(json::object_cref json_data)
{
    return (!json_data->contains("not_found") && json_data->contains("nickname"));
}

chat_user_info::chat_user_info(json::object_cref json_data) :
    sex_(chat_user::unknown),
    reg_time_(0),
    last_join_(0)
{
    if(!contains_user_info(json_data))
        throw exception("cannot create user info");

    nickname_ = json::get<std::wstring>(json_data, "nickname");
    fullname_ = json::get<std::wstring>(json_data, "fullname");
    birth_day_ = json::get<std::size_t>(json_data, "birth_day");
    birth_year_ = json::get<std::size_t>(json_data, "birth_year");
    birth_month_ = json::get<std::size_t>(json_data, "birth_month");
    location_ = json::get<std::wstring>(json_data, "location");
    homepage_ = json::get<std::wstring>(json_data, "homepage");
    icq_ = json::get<std::wstring>(json_data, "icq");
    email_ = json::get<std::wstring>(json_data, "email");
    phone_ = json::get<std::wstring>(json_data, "phone");
    eyes_ = json::get<std::wstring>(json_data, "eyes");
    hair_ = json::get<std::wstring>(json_data, "hair");
    height_ = json::get<std::wstring>(json_data, "height");
    meetings_ = json::get<std::wstring>(json_data, "meetings");
    knows_from_ = json::get<std::wstring>(json_data, "knows_from");
    about_ = json::get<std::wstring>(json_data, "about");
    reg_time_ = json::get<std::time_t>(json_data, "reg_time");
    last_join_ = json::get<std::time_t>(json_data, "last_join");

    photos_.update(json_data);

    if(json_data->contains("sex"))
        sex_ = json_data["sex"].as<chat_user::sex_type>();

    if(json_data->contains("vkontakte"))
        format::write(vkontakte_, json_data["vkontakte"].as<int>());
}

chat_user_info::~chat_user_info()
{
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
        format::print(uri, icq_uri, icq_);

    return utf8::convert(uri);
}

std::wstring chat_user_info::get_email_uri() const
{
    std::string uri;

    if(!email_.empty())
        format::print(uri, email_uri, email_);

    return utf8::convert(uri);
}

std::wstring chat_user_info::get_vkontakte_uri() const
{
    std::string uri;

    if(!vkontakte_.empty())
        format::print(uri, vkontakte_uri, vkontakte_);

    return utf8::convert(uri);
}

std::wstring chat_user_info::get_user_info_uri() const
{
    std::string uri;

    if(!nickname_.empty())
        format::print(uri, user_info_uri, nickname_);

    return utf8::convert(uri);
}

}   // namespace missio
