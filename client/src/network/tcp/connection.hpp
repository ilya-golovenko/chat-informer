//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_tcp_connection_hpp
#define _chat_network_tcp_connection_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/asio.hpp>


namespace chat
{
namespace net
{
namespace tcp
{

class connection
{
public:
    explicit connection(boost::asio::io_service& io_service);

    connection(connection const&) = delete;
    connection& operator=(connection const&) = delete;

    connection(connection&&) = default;
    connection& operator=(connection&&) = default;

    boost::asio::ip::tcp::socket& get_socket();
    boost::asio::io_service& get_io_service();

    boost::asio::ip::tcp::endpoint get_remote_endpoint() const;
    boost::asio::ip::tcp::endpoint get_local_endpoint() const;

    template <typename SettableSocketOption>
    void set_option(SettableSocketOption const& option);

    template <typename GettableSocketOption>
    void get_option(GettableSocketOption& option) const;

    template <typename ConnectHandler>
    void async_connect(boost::asio::ip::tcp::endpoint const& endpoint, ConnectHandler&& handler);

    template <typename MutableBufferSequence, typename ReadHandler>
    void async_read(MutableBufferSequence const& buffers, ReadHandler&& handler);

    template <typename ConstBufferSequence, typename WriteHandler>
    void async_write(ConstBufferSequence const& buffers, WriteHandler&& handler);

    bool is_open() const;
    void close();

private:
    boost::asio::ip::tcp::socket socket_;
};

template <typename SettableSocketOption>
void connection::set_option(SettableSocketOption const& option)
{
    socket_.set_option(option);
}

template <typename GettableSocketOption>
void connection::get_option(GettableSocketOption& option) const
{
    socket_.get_option(option);
}

template <typename ConnectHandler>
void connection::async_connect(boost::asio::ip::tcp::endpoint const& endpoint, ConnectHandler&& handler)
{
    socket_.async_connect(endpoint, std::forward<ConnectHandler>(handler));
}

template <typename MutableBufferSequence, typename ReadHandler>
void connection::async_read(MutableBufferSequence const& buffers, ReadHandler&& handler)
{
    socket_.async_read_some(buffers, std::forward<ReadHandler>(handler));
}

template <typename ConstBufferSequence, typename WriteHandler>
void connection::async_write(ConstBufferSequence const& buffers, WriteHandler&& handler)
{
    socket_.async_write_some(buffers, std::forward<WriteHandler>(handler));
}

}   // namespace tcp
}   // namespace net
}   // namespace chat

#endif  // _chat_network_tcp_connection_hpp
