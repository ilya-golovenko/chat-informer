//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_storage_hpp
#define _chat_core_storage_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/chat_user_cache.hpp>
#include <core/photoalbum.hpp>
#include <core/link_list.hpp>
#include <core/forum.hpp>
#include <core/news.hpp>
#include <core/version.hpp>
#include <core/info.hpp>
#include <core/events.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// BOOST headers
#include <boost/signals2.hpp>

// STL headers
#include <string>
#include <vector>


namespace chat
{

class storage
{
public:
    storage() = default;
    ~storage() = default;

    storage(storage const&) = delete;
    storage& operator=(storage const&) = delete;

    chat::news const& news() const;
    chat::forum const& forum() const;
    chat::link_list const& links() const;
    chat::chat_user_cache const& users() const;
    chat::photoalbum const& photoalbum() const;
    chat::version const& server_version() const;

    void update(missio::json::object const& json_data);

    void update_users();
    void clear_users();

    void add_alarm(std::wstring const& nickname);
    void remove_alarm(std::wstring const& nickname);
    void switch_alarm(std::wstring const& nickname);
    void clear_alarms();

    std::vector<std::wstring> get_alarms() const;
    void set_alarms(std::vector<std::wstring> const& nicknames);

    void fire_links_updated();

    template <typename Handler>
    void on_events_updated(Handler const& handler);

    template <typename Handler>
    void on_news_updated(Handler const& handler);

    template <typename Handler>
    void on_users_updated(Handler const& handler);

    template <typename Handler>
    void on_photoalbum_updated(Handler const& handler);

    template <typename Handler>
    void on_forum_updated(Handler const& handler);

    template <typename Handler>
    void on_links_updated(Handler const& handler);

private:
    void update_server_version(missio::json::object const& json_data);
    void update_photoalbum(missio::json::object const& json_data);
    void update_users(missio::json::object const& json_data);
    void update_links(missio::json::object const& json_data);
    void update_forum(missio::json::object const& json_data);
    void update_news(missio::json::object const& json_data);

private:
    boost::signals2::signal<void (chat::event::type)> events_updated_;

    boost::signals2::signal<void (chat::news const&)> news_updated_;
    boost::signals2::signal<void (chat::forum const&)> forum_updated_;
    boost::signals2::signal<void (chat::link_list const&)> links_updated_;
    boost::signals2::signal<void (chat::photoalbum const&)> photoalbum_updated_;
    boost::signals2::signal<void (chat::chat_user_cache const&)> users_updated_;

private:
    chat::news news_;
    chat::forum forum_;
    chat::link_list links_;
    chat::chat_user_cache users_;
    chat::photoalbum photoalbum_;
    chat::version server_version_;
};

template <typename Handler>
void storage::on_events_updated(Handler const& handler)
{
    events_updated_.connect(handler);
}

template <typename Handler>
void storage::on_news_updated(Handler const& handler)
{
    news_updated_.connect(handler);
}

template <typename Handler>
void storage::on_users_updated(Handler const& handler)
{
    users_updated_.connect(handler);
}

template <typename Handler>
void storage::on_photoalbum_updated(Handler const& handler)
{
    photoalbum_updated_.connect(handler);
}

template <typename Handler>
void storage::on_forum_updated(Handler const& handler)
{
    forum_updated_.connect(handler);
}

template <typename Handler>
void storage::on_links_updated(Handler const& handler)
{
    links_updated_.connect(handler);
}

}   // namespace chat

#endif  // _chat_core_storage_hpp
