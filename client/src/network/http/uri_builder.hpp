//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_uri_builder_hpp
#define _chat_network_http_uri_builder_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/uri/basic_uri_builder.hpp>
#include <network/http/uri_traits.hpp>


namespace chat
{
namespace net
{
namespace http
{

typedef basic_uri_builder<uri_traits> uri_builder;

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_uri_builder_hpp
