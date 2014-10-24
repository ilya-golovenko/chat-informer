//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_uri_traits_hpp
#define _chat_network_http_uri_traits_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/uri_traits.hpp>


namespace chat
{
namespace net
{
namespace http
{

bool uri_traits::is_valid(impl::uri_parts const& parts)
{
    return parts.host && !(*parts.host).empty();
}

bool uri_traits::is_default_port(impl::uri_parts const& parts)
{
    std::uint16_t const port = get_port(parts);
    std::string const scheme = get_scheme(parts);

    return (http_port_number == port && http_scheme_name == scheme) ||
           (https_port_number == port && https_scheme_name == scheme);
}

std::string uri_traits::scheme(impl::uri_parts const& parts)
{
    if(parts.scheme)
        return *parts.scheme;

    if(parts.port)
    {
        if(*parts.port == https_port_number)
            return https_scheme_name;
    }

    return http_scheme_name;
}

std::uint16_t uri_traits::port(impl::uri_parts const& parts)
{
    if(parts.port)
        return *parts.port;

    if(parts.scheme)
    {
        if(*parts.scheme == https_scheme_name)
            return https_port_number;
    }

    return http_port_number;
}

std::string uri_traits::path(impl::uri_parts const& parts)
{
    if(parts.path)
    {
        if((*parts.path)[0] != '/')
            return strings::slash + *parts.path;

        return *parts.path;
    }

    return strings::slash;
}

}   // namespace http
}   // namespace net
}   // namespace chat
