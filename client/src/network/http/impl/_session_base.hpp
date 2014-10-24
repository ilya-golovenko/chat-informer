//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_impl_session_base_hpp
#define _chat_network_http_impl_session_base_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/common.hpp>
#include <network/http/message.hpp>
#include <network/http/request.hpp>
#include <network/http/response.hpp>
#include <network/util/write_buffer.hpp>
#include <network/tcp/connection_timer.hpp>
#include <network/tcp/connection.hpp>

// BOOST headers
#include <boost/asio.hpp>

// STL headers
#include <cstdint>
#include <chrono>
#include <string>
#include <deque>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

class session_base
{
public:
    session_base(session_base const&) = delete;
    session_base& operator=(session_base const&) = delete;

    tcp::connection& get_tcp_connection();
    tcp::connection const& get_tcp_connection() const;

    std::chrono::seconds get_read_timeout() const;
    void set_read_timeout(std::chrono::seconds timeout);

    std::chrono::seconds get_keep_alive_timeout() const;
    void set_keep_alive_timeout(std::chrono::seconds timeout);

    void close(bool force);
    bool is_open() const;

protected:
    explicit session_base(tcp::connection&& tcp_connection);
    virtual ~session_base() = default;

    bool is_keep_alive() const;
    bool is_chunked_encoding() const;
    bool is_reading_completed() const;

    bool is_content_expected(request const& request) const;
    bool is_content_expected(response const& response) const;

protected:
    static bool is_keep_alive(message const& message);
    static bool is_chunked_encoding(message const& message);
    static std::uint64_t get_content_length(message const& message);

protected:
    bool writing_;
    bool closing_;
    bool keep_alive_;

    std::chrono::seconds read_timeout_;
    std::chrono::seconds keep_alive_timeout_;

    bool chunked_encoding_;
    std::size_t chunk_size_;
    std::string chunk_header_;

    bool content_expected_;
    std::uint64_t content_read_;
    std::uint64_t content_length_;

    tcp::connection tcp_connection_;
    tcp::connection_timer timeout_timer_;

    boost::asio::streambuf read_buffer_;
    std::deque<write_buffer> write_buffers_;
};

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_impl_session_base_hpp
