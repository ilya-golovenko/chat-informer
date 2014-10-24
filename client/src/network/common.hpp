//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//--------------------------------------------------------------------------- 
#ifndef _chat_network_common_hpp
#define _chat_network_common_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/range.hpp>
#include <boost/asio.hpp>

// STL headers
#include <functional>


namespace chat
{
namespace net
{

typedef boost::asio::streambuf::const_buffers_type const_buffers_type;
typedef boost::asio::buffers_iterator<const_buffers_type> buffers_iterator;
typedef boost::iterator_range<buffers_iterator> content_range;

typedef std::function<void (boost::system::error_code const&)> completion_handler;

}   // namespace net
}   // namespace chat

#endif  // _chat_network_common_hpp
