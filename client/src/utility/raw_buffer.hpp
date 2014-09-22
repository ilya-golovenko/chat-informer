//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_utility_raw_buffer_hpp
#define _chat_utility_raw_buffer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <cstddef>
#include <iosfwd>


namespace chat
{

class raw_buffer
{
public:
    typedef unsigned char value_type;
    typedef std::size_t size_type;
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef value_type* iterator;
    typedef value_type const* const_iterator;
    typedef value_type* pointer;
    typedef value_type const* const_pointer;

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
    size_type size() const;
    size_type capacity() const;

    pointer data();
    const_pointer data() const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    void clear();

    void grow(std::size_t size);
    void shrink(std::size_t size);
    void resize(std::size_t size);

    reference operator[](std::size_t index);
    const_reference operator[](std::size_t index) const;

private:
    void allocate(std::size_t size);

private:
    std::size_t data_size_;
    value_type* data_;

    std::size_t buffer_size_;
    value_type* buffer_;

    value_type small_buffer_[64];
};

bool operator<(raw_buffer const& lhs, raw_buffer const& rhs);
bool operator==(raw_buffer const& lhs, raw_buffer const& rhs);
bool operator!=(raw_buffer const& lhs, raw_buffer const& rhs);

std::ostream& operator<<(std::ostream& os, raw_buffer const& buffer);

}   // namespace chat

#endif  // _chat_utility_raw_buffer_hpp
