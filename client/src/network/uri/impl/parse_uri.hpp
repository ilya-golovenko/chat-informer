//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_impl_parse_uri_hpp
#define _chat_network_uri_impl_parse_uri_hpp

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

bool parse_uri(uri_parts& parts, std::string const& str);

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_impl_parse_uri_hpp
