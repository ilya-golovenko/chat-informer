//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/chat_user.hpp>


namespace chat
{

chat_user::chat_user(std::wstring const& nickname) :
    nickname_(nickname),
    sex_(unknown),
    online_(false),
    birthday_(false),
    alarm_(false)
{
}

bool chat_user::is_removeable() const
{
    return !online_ && !birthday_ && !alarm_;
}

std::wstring const& chat_user::nickname() const
{
    return nickname_;
}

void chat_user::set_sex(int sex)
{
    sex_ = static_cast<sex_type>(sex);
}

chat_user::sex_type chat_user::sex() const
{
    return sex_;
}

void chat_user::set_online(bool online)
{
    online_ = online;
}

void chat_user::set_birthday(bool birthday)
{
    birthday_ = birthday;
}

void chat_user::set_alarm(bool alarm)
{
    alarm_ = alarm;
}

bool chat_user::online() const
{
    return online_;
}

bool chat_user::birthday() const
{
    return birthday_;
}

bool chat_user::alarm() const
{
    return alarm_;
}

}   // namespace chat
