//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_request_hpp
#define _chat_network_http_request_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/uri.hpp>
#include <network/http/message.hpp>
#include <network/util/unique_buffer.hpp>
#include <network/util/shared_buffer.hpp>

// BOOST headers
#include <boost/optional.hpp>

// STL headers
#include <iostream>
#include <string>
#include <vector>


namespace chat
{
namespace net
{
namespace http
{

class request : public message
{
public:
    request();

    explicit request(version const& version);
    request(std::string const& method, uri const& uri);
    request(std::string const& method, uri const& uri, version const& version);

    request(request const&) = default;
    request& operator=(request const&) = default;

    request(request&&) = default;
    request& operator=(request&&) = default;

    uri const& get_uri() const;
    void set_uri(uri const& uri);

    std::string const& get_method() const;
    void set_method(std::string const& method);

    void set_host(std::string const& host);
    void set_host(std::string const& host, std::uint16_t port);
    boost::optional<std::string> get_host() const;

    void set_user_agent(std::string const& user_agent);
    boost::optional<std::string> get_user_agent() const;

    void set_cookie(std::string const& cookie);
    boost::optional<std::string> get_cookie() const;

    void serialize(std::ostream& os) const;

    unique_buffer to_unique_buffer() const;
    shared_buffer to_shared_buffer() const;

private:
    std::string method_;
    uri uri_;
};

inline std::ostream& operator<<(std::ostream& os, request const& request)
{
    request.serialize(os);
    return os;
}

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_request_hpp
