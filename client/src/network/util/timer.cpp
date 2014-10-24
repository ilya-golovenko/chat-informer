//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/util/timer.hpp>


namespace chat
{
namespace net
{
namespace util
{

timer::timer(boost::asio::io_service& io_service) :
    timer_(io_service)
{
}

void timer::cancel()
{
    timer_.cancel();
}

}   // namespace util
}   // namespace net
}   // namespace chat
