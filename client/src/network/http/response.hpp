//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_response_hpp
#define _chat_network_http_response_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/status.hpp>
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

class response : public message
{
public:
    response();

    explicit response(status status);
    response(version const& version, status const& status);

    response(response const&) = default;
    response& operator=(response const&) = default;

    response(response&&) = default;
    response& operator=(response&&) = default;

    status const& get_status() const;
    void set_status(status const& status);

    void set_server(std::string const& server);
    boost::optional<std::string> get_server() const;

    void set_cookie(std::string const& cookie);
    boost::optional<std::string> get_cookie() const;

    void serialize(std::ostream& os) const;

    unique_buffer to_unique_buffer() const;
    shared_buffer to_shared_buffer() const;

public:
    static response make_stock_response(status const& status, std::string const& server);

private:
    status status_;
};

inline std::ostream& operator<<(std::ostream& os, response const& response)
{
    response.serialize(os);
    return os;
}

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_response_hpp
