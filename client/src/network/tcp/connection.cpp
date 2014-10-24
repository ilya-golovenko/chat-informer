//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/tcp/connection.hpp>

// STL headers
#include <functional>


namespace chat
{
namespace net
{
namespace tcp
{

connection::connection(boost::asio::io_service& io_service) :
    socket_(io_service)
{
}

boost::asio::ip::tcp::socket& connection::get_socket()
{
    return socket_;
}

boost::asio::io_service& connection::get_io_service()
{
    return socket_.get_io_service();
}

boost::asio::ip::tcp::endpoint connection::get_remote_endpoint() const
{
    boost::system::error_code ignored_ec;
    return socket_.remote_endpoint(ignored_ec);
}

boost::asio::ip::tcp::endpoint connection::get_local_endpoint() const
{
    boost::system::error_code ignored_ec;
    return socket_.local_endpoint(ignored_ec);
}

bool connection::is_open() const
{
    return socket_.is_open();
}

void connection::close()
{
    if(socket_.is_open())
    {
        boost::asio::ip::tcp::socket::shutdown_type shutdown;
        shutdown = boost::asio::ip::tcp::socket::shutdown_both;

        boost::system::error_code ignored_ec;
        socket_.shutdown(shutdown, ignored_ec);

        socket_.close();
    }
}

}   // namespace tcp
}   // namespace net
}   // namespace chat
