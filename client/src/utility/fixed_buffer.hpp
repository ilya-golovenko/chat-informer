//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _utility_fixed_buffer_hpp
#define _utility_fixed_buffer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST header
#include <boost/noncopyable.hpp>

// STL headers
#include <cstddef>
#include <cstdlib>


namespace util
{

template <class T, std::size_t fixed_size = 256>
class fixed_buffer :
    private boost::noncopyable
{
public:
    explicit fixed_buffer(std::size_t size)
    {
        allocate(size * sizeof(T));
    }

    ~fixed_buffer()
    {
        if(buffer_ptr_ != reinterpret_cast<T*>(buffer_))
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
        if(size <= fixed_size)
            buffer_ptr_ = reinterpret_cast<T*>(buffer_);
        else
            buffer_ptr_ = reinterpret_cast<T*>(std::malloc(size));
    }

private:
    T* buffer_ptr_;
    char buffer_[fixed_size];
};

}   // namespace util

#endif  // _utility_fixed_buffer_hpp
