//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_impl_uri_traits_base_hpp
#define _chat_network_uri_impl_uri_traits_base_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/uri/impl/uri_parts.hpp>

// STL headers
#include <string>


namespace chat
{
namespace net
{
namespace impl
{

struct uri_traits_base
{
    uri_traits_base() = delete;
    ~uri_traits_base() = delete;

    static bool is_valid(uri_parts const& parts);

    static std::string scheme(uri_parts const& parts);
    static std::string user_info(uri_parts const& parts);
    static std::string host(uri_parts const& parts);
    static std::string path(uri_parts const& parts);
    static std::string query(uri_parts const& parts);
    static std::string fragment(uri_parts const& parts);
};

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_impl_uri_traits_base_hpp
