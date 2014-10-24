//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_util_shared_buffer_hpp
#define _chat_network_util_shared_buffer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/asio.hpp>

// STL headers
#include <cstdint>
#include <memory>
#include <string>


namespace chat
{
namespace net
{

class shared_buffer
{
public:
    shared_buffer();

    shared_buffer(void const* data, std::size_t size);

    shared_buffer(shared_buffer const&) = default;
    shared_buffer& operator=(shared_buffer const&) = default;

    shared_buffer(shared_buffer&&) = default;
    shared_buffer& operator=(shared_buffer&&) = default;

    boost::asio::const_buffers_1 const_buffer() const;

    explicit operator bool() const;

private:
    typedef std::shared_ptr<std::uint8_t> data_type;

private:
    std::size_t size_;
    data_type data_;
};

shared_buffer make_shared_buffer(char const* data);
shared_buffer make_shared_buffer(std::string const& data);

}   // namespace net
}   // namespace chat

#endif  // _chat_network_util_shared_buffer_hpp
