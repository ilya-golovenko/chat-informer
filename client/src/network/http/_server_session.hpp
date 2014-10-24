//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_server_session_hpp
#define _chat_network_http_server_session_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/impl/basic_session.hpp>

// STL headers
#include <string>


namespace chat
{
namespace net
{
namespace http
{

class server_session : public impl::basic_session<server_session>
{
public:
    static pointer create(tcp::connection&& tcp_connection);

public:
    explicit server_session(tcp::connection&& tcp_connection);

    void read_request();
    void write_response(response const& response);

private:
    void on_message_read(request const& request);
    void on_content_read(content_range const& content);
    void on_completed(boost::system::error_code const& error);
};

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_server_session_hpp
