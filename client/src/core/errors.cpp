//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/errors.hpp>

// MISSIO headers
#include <missio/utf8/convert.hpp>


namespace missio
{
namespace error
{

static char const* strings[] =
{
    "successful",
    "network I/O error",
    "bad HTTP response",
    "bad server data",
    "bad server status",
    "file I/O error",
    "operation aborted",
    "unknown error"
};

std::string to_string(type error)
{
    return std::string(strings[error]);
}

std::wstring to_wstring(type error)
{
    return utf8::convert(to_string(error));
}

}   // namespace error
}   // namespace missio
