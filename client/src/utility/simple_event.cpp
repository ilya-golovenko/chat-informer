//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <utility/simple_event.hpp>

// BOOST headers
#include <boost/bind.hpp>


namespace util
{

simple_event::simple_event() :
    flag_(false)
{
}

simple_event::~simple_event()
{
    set();
}

void simple_event::set()
{
    scoped_lock lock(mutex_);
    flag_ = true;

    lock.unlock();
    event_.notify_all();
}

void simple_event::reset()
{
    scoped_lock lock(mutex_);
    flag_ = false;
}

bool simple_event::wait(boost::posix_time::time_duration const& duration)
{
    scoped_lock lock(mutex_);

    return event_.timed_wait(
        lock, duration, boost::bind(
        &simple_event::wait_flag, this));
}

void simple_event::wait()
{
    scoped_lock lock(mutex_);

    event_.wait(lock, boost::bind(
        &simple_event::wait_flag, this));
}

bool simple_event::wait_flag() const
{
    return flag_;
}

}   // namespace util
