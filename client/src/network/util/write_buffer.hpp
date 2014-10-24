//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_util_write_buffer_hpp
#define _chat_network_util_write_buffer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/util/unique_buffer.hpp>
#include <network/util/shared_buffer.hpp>

// BOOST headers
#include <boost/asio.hpp>


namespace chat
{
namespace net
{

class write_buffer
{
public:
    explicit write_buffer(unique_buffer&& buffer);
    explicit write_buffer(shared_buffer const& buffer);

    write_buffer(write_buffer const& other) = delete;
    write_buffer& operator=(write_buffer const&) = delete;

    write_buffer(write_buffer&&) = default;
    write_buffer& operator=(write_buffer&&) = default;

    boost::asio::const_buffers_1 const_buffer() const;

private:
    unique_buffer unique_buffer_;
    shared_buffer shared_buffer_;
};

}   // namespace net
}   // namespace chat

#endif  // _chat_network_util_write_buffer_hpp
