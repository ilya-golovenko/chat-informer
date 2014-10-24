//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/storage.hpp>

// STL headers
#include <functional>


namespace chat
{

chat::news const& storage::news() const
{
    return news_;
}

chat::forum const& storage::forum() const
{
    return forum_;
}

chat::link_list const& storage::links() const
{
    return links_;
}

chat::chat_user_cache const& storage::users() const
{
    return users_;
}

chat::photoalbum const& storage::photoalbum() const
{
    return photoalbum_;
}

chat::version const& storage::server_version() const
{
    return server_version_;
}

void storage::update(missio::json::object const& json_data)
{
    update_server_version(json_data);
    update_photoalbum(json_data);
    update_users(json_data);
    update_links(json_data);
    update_forum(json_data);
    update_news(json_data);
}

void storage::update_users()
{
    events_updated_(event::userlist);
    users_updated_(std::cref(users_));
}

void storage::clear_users()
{
    users_.clear_cache();
    events_updated_(event::userlist);
    users_updated_(std::cref(users_));
}

void storage::add_alarm(std::wstring const& nickname)
{
    users_.add_alarm(nickname);
    events_updated_(event::userlist);
    users_updated_(std::cref(users_));
}

void storage::remove_alarm(std::wstring const& nickname)
{
    users_.remove_alarm(nickname);
    events_updated_(event::userlist);
    users_updated_(std::cref(users_));
}

void storage::switch_alarm(std::wstring const& nickname)
{
    users_.switch_alarm(nickname);
    events_updated_(event::userlist);
    users_updated_(std::cref(users_));
}

void storage::clear_alarms()
{
    users_.clear_alarms();
    events_updated_(event::userlist);
    users_updated_(std::cref(users_));
}

std::vector<std::wstring> storage::get_alarms() const
{
    return users_.get_alarms();
}

void storage::set_alarms(std::vector<std::wstring> const& nicknames)
{
    users_.set_alarms(nicknames);
    events_updated_(event::userlist);
    users_updated_(std::cref(users_));
}

void storage::fire_links_updated()
{
    events_updated_(event::links);
    links_updated_(std::cref(links_));
}

void storage::update_server_version(missio::json::object const& json_data)
{
    if(json_data.contains("server_info"))
    {
        int const major = json_data["server_info"]["version"]["major"];
        int const minor = json_data["server_info"]["version"]["minor"];
        int const build = json_data["server_info"]["version"]["build"];

        server_version_ = version(major, minor, build);

        if(server_version_ > informer_version)
            events_updated_(event::version);
    }
}

void storage::update_photoalbum(missio::json::object const& json_data)
{
    if(photoalbum_.update(json_data))
    {
        events_updated_(event::photoalbum);
        photoalbum_updated_(std::cref(photoalbum_));
    }
}

void storage::update_users(missio::json::object const& json_data)
{
    if(users_.update(json_data))
    {
        events_updated_(event::userlist);
        users_updated_(std::cref(users_));
    }
}

void storage::update_forum(missio::json::object const& json_data)
{
    if(forum_.update(json_data))
    {
        events_updated_(event::forum);
        forum_updated_(std::cref(forum_));
    }
}

void storage::update_links(missio::json::object const& json_data)
{
    if(links_.update(json_data))
    {
        events_updated_(event::links);
        links_updated_(std::cref(links_));
    }
}

void storage::update_news(missio::json::object const& json_data)
{
    if(news_.update(json_data))
    {
        events_updated_(event::news);
        news_updated_(std::cref(news_));
    }
}

}   // namespace chat
