//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _utility_simple_event_hpp
#define _utility_simple_event_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/noncopyable.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>


namespace util
{

class simple_event :
    private boost::noncopyable
{
public:
    /**
     * \brief Constructor.
     */
    simple_event();

    /**
     * \brief Destructor.
     */
    ~simple_event();

    /**
     * \brief Sets event to a signalled state.
     */
    void set();

    /**
     * \brief Sets event to a non-signalled state.
     */
    void reset();

    /**
     * \brief Waits for event to become signalled for a specified time interval.
     */
    bool wait(boost::posix_time::time_duration const& duration);
    
    /**
     * \brief Waits for event to become signalled for infinite time.
     */
    void wait();

private:
    bool wait_flag() const;

private:
    typedef boost::mutex::scoped_lock scoped_lock;
    typedef boost::condition_variable event_type;

private:
    volatile bool flag_;
    boost::mutex mutex_;
    event_type event_;
};

}   // namespace util

#endif  // _utility_simple_event_hpp
