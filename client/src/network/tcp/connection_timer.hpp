//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_tcp_connection_timer_hpp
#define _chat_network_tcp_connection_timer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/tcp/connection.hpp>
#include <network/util/timer.hpp>

// STL headers
#include <memory>
#include <chrono>


namespace chat
{
namespace net
{
namespace tcp
{

class connection_timer
{
public:
    explicit connection_timer(connection& connection);

    connection_timer(connection_timer const&) = delete;
    connection_timer& operator=(connection_timer const&) = delete;

    void start(std::chrono::seconds const& timeout);
    void cancel();

private:
    class impl : public std::enable_shared_from_this<impl>
    {
    public:
        explicit impl(connection_timer& owner);

        impl(impl const&) = delete;
        impl& operator=(impl const&) = delete;

        void start(std::chrono::seconds const& timeout);
        void cancel();

    private:
        void handle_timeout(boost::system::error_code const& error);

    private:
        connection& connection_;
        timer timeout_timer_;
    };

private:
    std::shared_ptr<impl> impl_;
};

}   // namespace tcp
}   // namespace net
}   // namespace chat

#endif  // _chat_network_tcp_connection_timer_hpp
