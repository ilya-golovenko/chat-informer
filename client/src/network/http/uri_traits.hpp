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
#include <network/uri/impl/uri_traits_base.hpp>

// STL headers
#include <string>


namespace chat
{
namespace net
{
namespace http
{

struct uri_traits : impl::uri_traits_base
{
    uri_traits() = delete;
    ~uri_traits() = delete;

    static bool is_valid(impl::uri_parts const& parts);

    static bool is_default_port(impl::uri_parts const& parts);

    static std::string scheme(impl::uri_parts const& parts);
    static std::uint16_t port(impl::uri_parts const& parts);
    static std::string path(impl::uri_parts const& parts);
};

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_uri_traits_hpp
