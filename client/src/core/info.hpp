//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_info_hpp
#define _missio_core_info_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <iostream>
#include <string>


namespace missio
{
namespace info
{

enum type
{
    auth,
    info,
    userlist,
    forum,
    links,
    birthdays,
    photoalbum,
    news,
    icons
};

std::string to_string(type info);
std::wstring to_wstring(type info);

inline std::ostream& operator<<(std::ostream& os, type info)
{
    os << to_string(info);
    return os;
}

}   // namespace info
}   // namespace missio

#endif  // _missio_core_info_hpp
