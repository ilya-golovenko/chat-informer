//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _net_http_proxy_settings_hpp
#define _net_http_proxy_settings_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/credentials.hpp>

// STL headers
#include <cstdint>
#include <string>


namespace chat
{
namespace net
{
namespace http
{

class proxy_settings
{
public:
    proxy_settings();

    proxy_settings(std::string const& hostname, std::uint16_t port);

    proxy_settings(std::string const& hostname, std::uint16_t port,
                   std::string const& username, std::string const& password);

    proxy_settings(proxy_settings const&) = default;
    proxy_settings& operator=(proxy_settings const&) = default;

    proxy_settings(proxy_settings&&) = default;
    proxy_settings& operator=(proxy_settings&&) = default;

    void set_proxy_server(std::string const& hostname, std::uint16_t port);
    void set_credentials(std::string const& username, std::string const& password);

    bool use_proxy() const;

    std::string const& get_hostname() const;
    std::uint16_t get_port() const;

    credentials const& get_credentials() const;

private:
    bool use_proxy_;
    std::string hostname_;
    std::uint16_t port_;
    credentials credentials_;
};

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _net_http_proxy_settings_hpp
