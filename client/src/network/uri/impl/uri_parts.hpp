//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_impl_uri_parts_hpp
#define _chat_network_uri_impl_uri_parts_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/optional.hpp>

// STL headers
#include <cstdint>
#include <string>


namespace chat
{
namespace net
{
namespace impl
{

struct uri_parts
{
    boost::optional<std::string> scheme;
    boost::optional<std::string> user_info;
    boost::optional<std::string> host;
    boost::optional<std::uint16_t> port;
    boost::optional<std::string> path;
    boost::optional<std::string> query;
    boost::optional<std::string> fragment;
};

bool operator==(uri_parts const& lhs, uri_parts const& rhs);
bool operator<(uri_parts const& lhs, uri_parts const& rhs); 

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_impl_uri_parts_hpp
