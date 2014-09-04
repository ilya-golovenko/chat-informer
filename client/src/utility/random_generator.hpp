//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
/**
 * \file utility/random_generator.hpp
 * \brief Defines thread-safe random generator class.
 */

#ifndef _chat_utility_random_generator_hpp
#define _chat_utility_random_generator_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

// STL headers
#include <ctime>


namespace chat
{

/**
 * \class random_generator
 * \brief Thread-safe random generator class.
 */
template <typename T>
class random_generator : private boost::noncopyable
{
public:
    typedef typename T::result_type result_type;

public:
    random_generator()
    {
        random_generator_.seed(std::time(0));
    }

    result_type operator()()
    {
        boost::mutex::scoped_lock lock(mutex_);
        return random_generator_();
    }

private:
    T random_generator_;
    boost::mutex mutex_;
};

}   // namespace chat

#endif  // _chat_utility_random_generator_hpp
