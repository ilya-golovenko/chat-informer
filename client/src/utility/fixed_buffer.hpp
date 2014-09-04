//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_utility_fixed_buffer_hpp
#define _chat_utility_fixed_buffer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST header
#include <boost/static_assert.hpp>
#include <boost/noncopyable.hpp>

// STL headers
#include <cstddef>
#include <cstdlib>
#include <new>


namespace chat
{

template <class T, std::size_t fixed_size = 256>
class fixed_buffer : private boost::noncopyable
{
// T should be a POD (Plain Old Data) type
BOOST_STATIC_ASSERT(boost::is_pod<T>::value);

public:
    explicit fixed_buffer(std::size_t size)
    {
        allocate(size * sizeof(T));
    }

    fixed_buffer() :
        buffer_ptr_(buffer)
    {
    }

    ~fixed_buffer()
    {
        if(reinterpret_cast<T*>(buffer_) != buffer_ptr_)
            std::free(buffer_ptr_);
    }

    operator T*() const
    {
        return buffer_ptr_;
    }

    T* operator&() const
    {
        return buffer_ptr_;
    }

private:
    void allocate(std::size_t size)
    {
        if(size > sizeof(buffer_))
        {
          void* buffer = std::malloc(size);

          if(!buffer)
              throw std::bad_alloc();

          buffer_ptr_ = reinterpret_cast<T*>(buffer);
        }
        else
        {
          buffer_ptr_ = reinterpret_cast<T*>(buffer_);
        }
    }

private:
    T* buffer_ptr_;
    char buffer_[fixed_size];
};

}   // namespace chat

#endif  // _chat_utility_fixed_buffer_hpp
