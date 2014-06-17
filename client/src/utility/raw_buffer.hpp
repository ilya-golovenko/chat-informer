//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _utility_raw_buffer_hpp
#define _utility_raw_buffer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <cstddef>
#include <iosfwd>


namespace util
{

class raw_buffer
{
public:
    typedef unsigned char value_type;

    typedef value_type* iterator;
    typedef value_type const* const_iterator;

public:
    raw_buffer();
    ~raw_buffer();

    explicit raw_buffer(std::size_t size);
    raw_buffer(void const* data, std::size_t size);

    raw_buffer(raw_buffer const& other);
    raw_buffer& operator=(raw_buffer const& other);

    void assign(raw_buffer const& other);
    void assign(void const* data, std::size_t size);

    void append(raw_buffer const& other);
    void append(void const* data, std::size_t size);

    bool empty() const;
    std::size_t size() const;
    std::size_t capacity() const;

    value_type* data();
    value_type const* data() const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    void clear();

    void grow(std::size_t size);
    void shrink(std::size_t size);
    void resize(std::size_t size);

    value_type& operator[](std::size_t index);
    value_type const& operator[](std::size_t index) const;

    bool operator<(raw_buffer const& other) const;
    bool operator==(raw_buffer const& other) const;
    bool operator!=(raw_buffer const& other) const;

private:
    void initialize();
    void allocate(std::size_t size);

private:
    std::size_t size_;
    value_type* buffer_ptr_;

    value_type* buffer_;
    std::size_t buffer_size_;

    value_type small_buffer_[64];
};

std::ostream& operator<<(std::ostream& os, raw_buffer const& buffer);

}   // namespace util

#endif  // _utility_raw_buffer_hpp
