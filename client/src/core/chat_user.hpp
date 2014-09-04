//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_chat_user_hpp
#define _chat_core_chat_user_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <string>


namespace chat
{

class chat_user
{
public:
    enum sex_type
    {
        unknown = -2,
        guest   = -1,
        hidden  = 0,
        male    = 1,
        female  = 2
    };

public:
    explicit chat_user(std::wstring const& nickname);
    ~chat_user();

    chat_user(chat_user&& other);
    chat_user& operator=(chat_user&& other);

    chat_user(chat_user const& other);
    chat_user& operator=(chat_user const& other);

    bool is_removeable() const;
    std::wstring const& nickname() const;

    void set_sex(int sex);
    sex_type sex() const;

    void set_online(bool online);
    void set_birthday(bool birthday);
    void set_alarm(bool alarm);

    bool online() const;
    bool birthday() const;
    bool alarm() const;

private:
    std::wstring nickname_;
    sex_type sex_;
    bool online_;
    bool birthday_;
    bool alarm_;
};

}   // namespace chat

#endif  // _chat_core_chat_user_hpp
