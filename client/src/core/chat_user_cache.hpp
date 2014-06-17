//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_chat_user_cache_hpp
#define _missio_core_chat_user_cache_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/chat_user_list.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// BOOST headers
#include <boost/noncopyable.hpp>

// STL headers
#include <string>
#include <vector>


namespace missio
{

class chat_user_cache :
    private boost::noncopyable
{
public:
    chat_user_cache();
    ~chat_user_cache();

    void clear_cache();
    void cleanup_cache();

    bool update(json::object_cref json_data);

    chat_user_list get_alarm_users() const;
    chat_user_list get_online_users() const;
    chat_user_list get_birthday_users() const;

    std::wstring const& get_birthdays_today() const;

    void add_alarm(std::wstring const& nickname);
    void remove_alarm(std::wstring const& nickname);
    void switch_alarm(std::wstring const& nickname);
    void clear_alarms();

    std::vector<std::wstring> get_alarms() const;
    void set_alarms(std::vector<std::wstring> const& nicknames);

    bool contains(std::wstring const& nickname) const;
    std::size_t index_of(std::wstring const& nickname) const;

    chat_user const& operator[](std::size_t index) const;
    chat_user const& operator[](std::wstring const& nickname) const;

    //void subscribe_to_user_alarm();
    //void subscribe_to_user_birthday();

public:
    static std::size_t const invalid_index = chat_user_list::invalid_index;

private:
    void clear_alarm_users();
    void clear_online_users();
    void clear_birthday_users();

    chat_user& mutable_get(std::wstring const& nickname);
    chat_user& creating_get(std::wstring const& nickname);

private:
     std::wstring today_;
     chat_user_list users_;
};

}   // namespace missio

#endif  // _missio_core_chat_user_cache_hpp
