//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_server_connection_hpp
#define _chat_network_http_server_connection_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Applicationg headers
#include <http/response.hpp>
#include <http/request.hpp>
#include <http/common.hpp>
#include <http/buffer.hpp>
#include <http/chunked_parser.hpp>
#include <http/request_parser.hpp>
#include <http/tcp_connection.hpp>

// BOOST headers
#include <boost/asio/steady_timer.hpp>
#include <boost/asio.hpp>

// STL headers
#include <memory>
#include <string>
#include <chrono>
#include <queue>


namespace http
{

class server_connection : public std::enable_shared_from_this<server_connection>
{
public:
    typedef std::shared_ptr<server_connection> pointer;

public:
    static pointer create(tcp::connection&& tcp_connection);

public:
    explicit server_connection(tcp::connection&& tcp_connection);
    ~server_connection();

    server_connection(server_connection const&) = delete;
    server_connection& operator=(server_connection const&) = delete;

    request const& get_request() const;

    tcp::connection& get_tcp_connection();
    tcp::connection const& get_tcp_connection() const;

    boost::asio::ip::tcp::endpoint get_local_endpoint() const;
    boost::asio::ip::tcp::endpoint get_remote_endpoint() const;

    void read_request(completion_handler const& handler);
    void read_request(completion_handler const& handler, std::chrono::seconds const& timeout);

    void write_stock_response(status const& status, std::string const& server, completion_handler const& handler);

    void write_response(response const& response, completion_handler const& handler);
    void write_buffer(buffer const& buffer, completion_handler const& handler);

    void close(status const& status, std::string const& server, completion_handler const& handler);

    void close(response const& response, completion_handler const& handler);
    void close(buffer const& buffer, completion_handler const& handler);

    void close(bool force);
    bool is_open() const;

private:
    void close_connection(bool force);
    void write_front_connection_buffer();

    void setup_connection(boost::system::error_code const& error);
    bool is_reading_completed(boost::system::error_code const& error) const;

    void call_completion_handler(boost::system::error_code const& error);

    void handle_stop(boost::system::error_code const& error);
    void handle_read(boost::system::error_code const& error, std::size_t bytes_transferred);
    void handle_write(boost::system::error_code const& error, std::size_t bytes_transferred);

private:
    bool reading_body_;
    bool writing_response_;
    bool closing_connection_;
    bool chunked_encoding_;

    std::size_t chunk_size_;

    buffer buffer_;
    request request_;
    response response_;

    tcp_connection tcp_connection_;

    std::queue<buffer> write_buffers_;
    std::queue<completion_handler> handlers_;

    boost::asio::steady_timer timeout_timer_;

    chunked_parser chunked_parser_;
    request_parser request_parser_;
};

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_server_connection_hpp
