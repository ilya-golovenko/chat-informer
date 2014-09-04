//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_events_hpp
#define _chat_core_events_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// SLT headers
#include <iostream>
#include <string>


namespace chat
{
namespace event
{

enum type
{
    none,
    version,
    userlist,
    forum,
    photoalbum,
    links,
    news
};

std::string to_string(type event);
std::wstring to_wstring(type event);

inline std::ostream& operator<<(std::ostream& os, type event)
{
    os << to_string(event);
    return os;
}

}   // namespace event
}   // namespace chat

#endif  // _chat_core_events_hpp
