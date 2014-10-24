//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/uri/impl/uri_traits_base.hpp>


namespace chat
{
namespace net
{
namespace impl
{

bool uri_traits_base::valid(uri_parts const& parts)
{
    return true;
}

std::string uri_traits_base::scheme(uri_parts const& parts)
{
    return parts.scheme ? *parts.scheme : std::string();
}

std::string uri_traits_base::user_info(uri_parts const& parts)
{
    return parts.user_info ? *parts.user_info : std::string();
}

std::string uri_traits_base::host(uri_parts const& parts)
{
    return parts.host ? *parts.host : std::string();
}

std::string uri_traits_base::path(uri_parts const& parts)
{
    return parts.path ? *parts.path : std::string();
}

std::string uri_traits_base::query(uri_parts const& parts)
{
    return parts.query ? *parts.query : std::string();
}

std::string uri_traits_base::fragment(uri_parts const& parts)
{
    return parts.fragment ? *parts.fragment : std::string();
}

}   // namespace impl
}   // namespace net
}   // namespace chat
