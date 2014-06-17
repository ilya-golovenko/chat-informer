//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/events.hpp>

// MISSIO headers
#include <missio/utf8/convert.hpp>


namespace missio
{
namespace event
{

static char const* strings[] =
{
    "none",
    "version",
    "userlist",
    "forum",
    "photoalbum",
    "links",
    "news"
};

std::string to_string(type event)
{
    return std::string(strings[event]);
}

std::wstring to_wstring(type event)
{
    return utf8::convert(to_string(event));
}

}   // namespace event
}   // namespace missio
