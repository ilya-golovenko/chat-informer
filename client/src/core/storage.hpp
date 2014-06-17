//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_storage_hpp
#define _missio_core_storage_hpp

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
#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>

// STL headers
#include <string>
#include <vector>


namespace missio
{

class storage :
    private boost::noncopyable
{
public:
    storage();
    ~storage();

    missio::news const& news() const;
    missio::forum const& forum() const;
    missio::link_list const& links() const;
    missio::chat_user_cache const& users() const;
    missio::photoalbum const& photoalbum() const;
    missio::version const& server_version() const;

    void update(json::object_cref json_data);

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
    void update_server_version(json::object_cref json_data);
    void update_photoalbum(json::object_cref json_data);
    void update_users(json::object_cref json_data);
    void update_links(json::object_cref json_data);
    void update_forum(json::object_cref json_data);
    void update_news(json::object_cref json_data);

private:
    boost::signals2::signal<void (missio::event::type)> events_updated_;

    boost::signals2::signal<void (missio::news const&)> news_updated_;
    boost::signals2::signal<void (missio::forum const&)> forum_updated_;
    boost::signals2::signal<void (missio::link_list const&)> links_updated_;
    boost::signals2::signal<void (missio::photoalbum const&)> photoalbum_updated_;
    boost::signals2::signal<void (missio::chat_user_cache const&)> users_updated_;

private:
    missio::news news_;
    missio::forum forum_;
    missio::link_list links_;
    missio::chat_user_cache users_;
    missio::photoalbum photoalbum_;
    missio::version server_version_;
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

}   // namespace core

#endif  // _missio_core_storage_hpp
