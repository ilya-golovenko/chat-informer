//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/chat_user_cache.hpp>
#include <core/exception.hpp>

// BOOST headers
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/next_prior.hpp>
#include <boost/bind.hpp>


namespace chat
{

void chat_user_cache::clear_cache()
{
    clear_online_users();
    cleanup_cache();
}

void chat_user_cache::cleanup_cache()
{
    boost::remove_erase_if(users_, boost::bind(&chat_user::is_removeable, _1));
}

bool chat_user_cache::update(missio::json::object const& json_data)
{
    bool updated = false;

    if(json_data.contains("userlist"))
    {
        clear_online_users();

        missio::json::array const& json_userlist = json_data["userlist"];

        for(missio::json::object const& json_user : json_userlist)
        {
            std::wstring const nickname = json_user["nickname"];
            chat_user::sex_type const sex = json_user["sex"];

            chat_user& user = creating_get(nickname);

            user.set_online(true);
            user.set_sex(sex);
        }

        updated = true;
    }

    if(json_data.contains("birthdays"))
    {
        clear_birthday_users();

        missio::json::object const& json_birthdays = json_data["birthdays"];
        missio::json::array const& json_nicknames = json_birthdays["nicknames"];

        today_ = json_birthdays["today"].as<std::wstring>();

        for(std::wstring const nickname : json_nicknames[index])
        {
             creating_get(nickname).set_birthday(true);
        }

        updated = true;
    }

    if(updated)
        cleanup_cache();

    return updated;
}

chat_user_list chat_user_cache::get_alarm_users() const
{
    chat_user_list::vector_type alarm_users;

    boost::push_back(alarm_users, users_
        | boost::adaptors::filtered(boost::bind(&chat_user::alarm, _1))); 

    return chat_user_list(std::move(alarm_users));
}

chat_user_list chat_user_cache::get_online_users() const
{
    chat_user_list::vector_type online_users;

    boost::push_back(online_users, users_
        | boost::adaptors::filtered(boost::bind(&chat_user::online, _1))); 

    return chat_user_list(std::move(online_users));
}

chat_user_list chat_user_cache::get_birthday_users() const
{
    chat_user_list::vector_type birthday_users;

    boost::push_back(birthday_users, users_
        | boost::adaptors::filtered(boost::bind(&chat_user::birthday, _1))); 

    return chat_user_list(std::move(birthday_users));
}

std::wstring const& chat_user_cache::get_birthdays_today() const
{
    return today_;
}

void chat_user_cache::add_alarm(std::wstring const& nickname)
{
    creating_get(nickname).set_alarm(true);
}

void chat_user_cache::remove_alarm(std::wstring const& nickname)
{
    chat_user& user = mutable_get(nickname);

    if(user.alarm())
    {
        user.set_alarm(false);

        if(user.is_removeable())
            users_.erase(nickname);
    }
}

void chat_user_cache::switch_alarm(std::wstring const& nickname)
{
    chat_user& user = mutable_get(nickname);

    if(!user.alarm())
        user.set_alarm(true);
    else
        remove_alarm(nickname);
}

void chat_user_cache::clear_alarms()
{
    clear_alarm_users();
    cleanup_cache();
}

std::vector<std::wstring> chat_user_cache::get_alarms() const
{
    std::vector<std::wstring> nicknames;

    boost::push_back(nicknames, users_
        | boost::adaptors::filtered(boost::bind(&chat_user::alarm, _1))
        | boost::adaptors::transformed(boost::bind(&chat_user::nickname, _1)));

    return nicknames;
}

void chat_user_cache::set_alarms(std::vector<std::wstring> const& nicknames)
{
    clear_alarm_users();

    boost::for_each(nicknames, boost::bind(&chat_user_cache::add_alarm, this, _1));
}

bool chat_user_cache::contains(std::wstring const& nickname) const
{
    return users_.contains(nickname);
}

std::size_t chat_user_cache::index_of(std::wstring const& nickname) const
{
    return users_.index_of(nickname);
}

chat_user const& chat_user_cache::operator[](std::size_t index) const
{
    return users_[index];
}

chat_user const& chat_user_cache::operator[](std::wstring const& nickname) const
{
    return users_[nickname];
}

void chat_user_cache::clear_alarm_users()
{
    boost::for_each(users_, boost::bind(&chat_user::set_alarm, _1, false));
}

void chat_user_cache::clear_online_users()
{
    boost::for_each(users_, boost::bind(&chat_user::set_online, _1, false));
}

void chat_user_cache::clear_birthday_users()
{
    boost::for_each(users_, boost::bind(&chat_user::set_birthday, _1, false));
}

chat_user& chat_user_cache::mutable_get(std::wstring const& nickname)
{
    chat_user_list::iterator it = users_.find(nickname);

    if(it == users_.end())
        throw exception(L"cannot find user: " + nickname);

    return *it;
}

chat_user& chat_user_cache::creating_get(std::wstring const& nickname)
{
    chat_user_list::iterator it = users_.find(nickname);

    if(it == users_.end())
    {
        users_.push_back(chat_user(nickname));
        it = boost::prior(users_.end());
    }

    return *it;
}

}   // namespace chat
