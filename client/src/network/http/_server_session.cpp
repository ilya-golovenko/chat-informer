//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/server_session.hpp>

// STL headers
#include <utility>


namespace chat
{
namespace net
{
namespace http
{

server_session::pointer server_session::create(tcp::connection&& tcp_connection)
{
    return std::make_shared<server_session>(std::forward<tcp::connection>(tcp_connection));
}

server_session::server_session(tcp::connection&& tcp_connection) :
    basic_session(std::forward<tcp::connection>(tcp_connection))
{
}

void server_session::read_request()
{
    begin_read_message<request>();
}

void server_session::write_response(response const& response)
{
    begin_write_message(response);
}

}   // namespace http
}   // namespace net
}   // namespace chat
