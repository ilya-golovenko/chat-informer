//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/proxy_settings.hpp>


namespace chat
{
namespace net
{
namespace http
{

proxy_settings::proxy_settings() :
    use_proxy_(false)
{
}

proxy_settings::proxy_settings(std::string const& hostname, std::uint16_t port) :
    use_proxy_(true),
    hostname_(hostname),
    port_(port)
{
}

proxy_settings::proxy_settings(std::string const& hostname, std::uint16_t port,
                               std::string const& username, std::string const& password) :
    credentials_(username, password),
    use_proxy_(true),
    hostname_(hostname),
    port_(port)
{
}

void proxy_settings::set_proxy_server(std::string const& hostname, std::uint16_t port)
{
    use_proxy_ = true;
    hostname_ = hostname;
    port_ = port;
}

void proxy_settings::set_credentials(std::string const& username, std::string const& password)
{
    credentials_.set_username(username);
    credentials_.set_password(password);
}

bool proxy_settings::use_proxy() const
{
    return use_proxy_;
}

std::string const& proxy_settings::get_hostname() const
{
    return hostname_;
}

std::uint16_t proxy_settings::get_port() const
{
    return port_;
}

credentials const& proxy_settings::get_credentials() const
{
    return credentials_;
}

}   // namespace http
}   // namespace net
}   // namespace chat
