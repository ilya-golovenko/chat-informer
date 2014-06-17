//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/info.hpp>

// MISSIO headers
#include <missio/utf8/convert.hpp>


namespace missio
{
namespace info
{

static char const* strings[] =
{
    "auth",
    "info",
    "userlist",
    "forum",
    "links",
    "birthdays",
    "photoalbum",
    "news",
    "icons"
};

std::string to_string(type info)
{
    return std::string(strings[info]);
}

std::wstring to_wstring(type info)
{
    return utf8::convert(to_string(info));
}

}   // namespace info
}   // namespace missio
