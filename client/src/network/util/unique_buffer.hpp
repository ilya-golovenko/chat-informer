//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_util_unique_buffer_hpp
#define _chat_network_util_unique_buffer_hpp

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

class unique_buffer
{
public:
    unique_buffer();

    unique_buffer(void const* data, std::size_t size);

    unique_buffer(unique_buffer const&) = delete;
    unique_buffer& operator=(unique_buffer const&) = delete;

    unique_buffer(unique_buffer&&) = default;
    unique_buffer& operator=(unique_buffer&&) = default;

    boost::asio::const_buffers_1 const_buffer() const;

    explicit operator bool() const;

private:
    typedef std::unique_ptr<std::uint8_t[]> data_type;

private:
    std::size_t size_;
    data_type data_;
};

unique_buffer make_unique_buffer(char const* data);
unique_buffer make_unique_buffer(std::string const& data);

}   // namespace net
}   // namespace chat

#endif  // _chat_network_util_unique_buffer_hpp
