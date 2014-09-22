//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <utility/raw_buffer.hpp>
#include <utility/next_power_of_two.hpp>

// STL headers
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cctype>


namespace chat
{

raw_buffer::raw_buffer() :
    data_size_(0u),
    data_(nullptr),
    buffer_size_(0u),
    buffer_(nullptr)
{
}

raw_buffer::~raw_buffer()
{
    std::free(buffer_);
}

raw_buffer::raw_buffer(std::size_t size) : raw_buffer()
{
    allocate(size);
}

raw_buffer::raw_buffer(void const* data, std::size_t size) : raw_buffer()
{
    assign(data, size);
}

raw_buffer::raw_buffer(raw_buffer const& other) : raw_buffer()
{
    assign(other);
}

raw_buffer& raw_buffer::operator=(raw_buffer const& other)
{
    assign(other);
    return *this;
}

void raw_buffer::assign(raw_buffer const& other)
{
    if(&other != this)
        assign(other.data_, other.data_size_);
}

void raw_buffer::assign(void const* data, std::size_t size)
{
    allocate(size);
    std::memcpy(data_, data, size);
}

void raw_buffer::append(raw_buffer const& other)
{
    append(other.data(), other.size());
}

void raw_buffer::append(void const* data, std::size_t size)
{
    grow(size);
    std::memcpy(data_ + data_size_ - size, data, size);
}

bool raw_buffer::empty() const
{
    return 0u == data_size_;
}

raw_buffer::size_type raw_buffer::size() const
{
    return data_size_;
}

raw_buffer::size_type raw_buffer::capacity() const
{
    return std::max(buffer_size_, sizeof(small_buffer_));
}

raw_buffer::pointer raw_buffer::data()
{
    return data_;
}

raw_buffer::const_pointer raw_buffer::data() const
{
    return data_;
}

raw_buffer::iterator raw_buffer::begin()
{
    return data_;
}

raw_buffer::iterator raw_buffer::end()
{
    return data_ + data_size_;
}

raw_buffer::const_iterator raw_buffer::begin() const
{
    return data_;
}

raw_buffer::const_iterator raw_buffer::end() const
{
    return data_ + data_size_;
}

void raw_buffer::clear()
{
    std::free(buffer_);

    data_size_ = 0u;
    data_ = nullptr;

    buffer_size_ = 0u;
    buffer_ = nullptr;
}

void raw_buffer::grow(std::size_t size)
{
    allocate(data_size_ + size);
}

void raw_buffer::shrink(std::size_t size)
{
    allocate(data_size_ > size ? data_size_ - size : 0u);
}

void raw_buffer::resize(std::size_t size)
{
    allocate(size);
}

raw_buffer::reference raw_buffer::operator[](std::size_t index)
{
    return data_[index];
}

raw_buffer::const_reference raw_buffer::operator[](std::size_t index) const
{
    return data_[index];
}

void raw_buffer::allocate(std::size_t size)
{
    if(size > sizeof(small_buffer_))
    {
        if(size > buffer_size_)
        {
            std::size_t const new_size = next_power_of_two(size);
            void* new_buffer = std::realloc(buffer_, new_size);

            if(!new_buffer)
                throw std::bad_alloc();

            buffer_ = static_cast<value_type*>(new_buffer);

            buffer_size_ = new_size;
            data_ = buffer_;
        }

        if(data_size_ > 0u && data_size_ <= sizeof(small_buffer_))
            std::memcpy(data_, small_buffer_, data_size_);
    }
    else
    {
        data_ = size > 0u ? small_buffer_ : nullptr;

        if(buffer_)
        {
            if(size > 0u && data_size_ > sizeof(small_buffer_))
                std::memcpy(data_, buffer_, size);

            std::free(buffer_);

            buffer_size_ = 0u;
            buffer_ = nullptr;
        }
    }

    data_size_ = size;
}

bool operator<(raw_buffer const& lhs, raw_buffer const& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

bool operator==(raw_buffer const& lhs, raw_buffer const& rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

bool operator!=(raw_buffer const& lhs, raw_buffer const& rhs)
{
    return !operator==(lhs, rhs);
}

std::ostream& operator<<(std::ostream& os, raw_buffer const& buffer)
{
    bool has_non_printable = false;

    for(auto const value : buffer)
    {
        if(!std::isprint(value))
        {
            has_non_printable = true;
            break;
        }
    }

    if(has_non_printable)
    {
        std::ios state(nullptr);
        state.copyfmt(os);

        auto const old_flags = os.flags();

        os.unsetf(std::ios::showbase);
        os.setf(std::ios::uppercase);

        for(int const value : buffer)
        {
            os << std::setfill('0')
               << std::setw(2)
               << std::hex
               << "\\x"
               << value;
        }

        os.copyfmt(state);
    }
    else
    {
        for(char const value : buffer)
        {
            os << value;
        }
    }

    return os;
}

}   // namespace chat
