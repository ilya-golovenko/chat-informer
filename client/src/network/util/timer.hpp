//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_util_timer_hpp
#define _chat_network_util_timer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/asio/steady_timer.hpp>

// STL headers
#include <utility>


namespace chat
{
namespace net
{

class timer
{
public:
    explicit timer(boost::asio::io_service& io_service);

    timer(timer const&) = delete;
    timer& operator=(timer const&) = delete;

    timer(timer&&) = default;
    timer& operator=(timer&&) = default;

    template <typename Duration, typename Handler>
    void async_wait(Duration const& duration, Handler&& handler);

    void cancel();

private:
    boost::asio::steady_timer timer_;
};

template <typename Duration, typename Handler>
void timer::async_wait(Duration const& duration, Handler&& handler)
{
    timer_.expires_from_now(duration);
    timer_.async_wait(std::forward<Handler>(handler));
}

}   // namespace net
}   // namespace chat

#endif  // _chat_network_util_timer_hpp
