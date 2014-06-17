//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _utility_singleton_hpp
#define _utility_singleton_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>


namespace util
{

template <typename T>
class singleton :
    private boost::noncopyable
{
public:
    static T& instance();

private:
    singleton();
    ~singleton();

private:
    static void init();

private:
    static boost::once_flag flag_;
    static boost::scoped_ptr<T> instance_;
};

template <typename T>
T& singleton<T>::instance()
{
    boost::call_once(init, flag_);
    return *instance_;
}

template <typename T>
void singleton<T>::init()
{
    instance_.reset(new T());
}

template <typename T>
boost::once_flag singleton<T>::flag_ = BOOST_ONCE_INIT;

template <typename T>
boost::scoped_ptr<T> singleton<T>::instance_;

}   // namespace util

#endif  // _utility_singleton_hpp
