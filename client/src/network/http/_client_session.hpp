//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_client_session_hpp
#define _chat_network_http_client_session_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/impl/basic_session.hpp>
#include <network/http/proxy_settings.hpp>

// STL headers
#include <cstdint>
#include <string>


namespace chat
{
namespace net
{
namespace http
{

class client_session : public impl::basic_session<client_session>
{
public:
    static pointer create(boost::asio::io_service& io_service);

public:
    explicit client_session(boost::asio::io_service& io_service);

    void set_proxy_settings(proxy_settings const& proxy_settings);

    void write_request(std::string const& host, std::uint16_t port, request&& request);
    void write_request(uri const& uri, request&& request);

    void read_response();

private:
    void open_session(std::string const& host, std::uint16_t port, bool redirected);
    void restart_session();

    bool follow_redirect();
    bool auth_proxy_request();

    bool is_keep_alive_error(boost::system::error_code const& error) const;
    bool is_keep_alive_aborted(boost::system::error_code const& error) const;

    void begin_resolve(std::string const& host, std::uint16_t port);
    void begin_connect(boost::asio::ip::tcp::resolver::iterator iterator);

    void handle_resolve(boost::system::error_code const& error, boost::asio::ip::tcp::resolver::iterator iterator);
    void handle_connect(boost::system::error_code const& error, boost::asio::ip::tcp::resolver::iterator iterator);

private:
    void on_message_read(response const& response);
    void on_completed(boost::system::error_code const& error);

private:
    //static authentication::pointer create_proxy_auth(std::vector<std::string> const& schemas);

private:
    std::string host_;
    std::uint16_t port_;

    request request_;
    response response_;

    std::size_t redirect_count_;

    proxy_settings proxy_settings_;
    //authentication::pointer proxy_auth_;

    boost::asio::ip::tcp::resolver resolver_;
};

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_client_session_hpp
