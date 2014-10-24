//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_impl_uri_encode_hpp
#define _chat_network_uri_impl_uri_encode_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <string>


namespace chat
{
namespace net
{
namespace impl
{

std::string encode_user_info(std::string const& user_info);
std::string encode_host(std::string const& host);
std::string encode_path(std::string const& path);
std::string encode_query(std::string const& query);
std::string encode_fragment(std::string const& fragment);

std::string decode(std::string const& str);

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_impl_uri_encode_hpp
