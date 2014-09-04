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
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cctype>


namespace chat
{

raw_buffer::raw_buffer()
{
    initialize();
}

raw_buffer::~raw_buffer()
{
    std::free(buffer_);
}

raw_buffer::raw_buffer(std::size_t size)
{
    initialize();
    allocate(size);
}

raw_buffer::raw_buffer(void const* data, std::size_t size)
{
    initialize();
    assign(data, size);
}

raw_buffer::raw_buffer(raw_buffer const& other)
{
    initialize();
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
        assign(other.buffer_ptr_, other.size_);
}

void raw_buffer::assign(void const* data, std::size_t size)
{
    allocate(size);
    std::memcpy(buffer_ptr_, data, size);
}

void raw_buffer::append(raw_buffer const& other)
{
    append(other.data(), other.size());
}

void raw_buffer::append(void const* data, std::size_t size)
{
    grow(size);
    std::memcpy(end() - size, data, size);
}

bool raw_buffer::empty() const
{
    return !size_;
}

std::size_t raw_buffer::size() const
{
    return size_;
}

std::size_t raw_buffer::capacity() const
{
    return std::max(buffer_size_, sizeof(small_buffer_));
}

raw_buffer::value_type* raw_buffer::data()
{
    return buffer_ptr_;
}

raw_buffer::value_type const* raw_buffer::data() const
{
    return buffer_ptr_;
}

raw_buffer::iterator raw_buffer::begin()
{
    return buffer_ptr_;
}

raw_buffer::iterator raw_buffer::end()
{
    return (buffer_ptr_ + size_);
}

raw_buffer::const_iterator raw_buffer::begin() const
{
    return buffer_ptr_;
}

raw_buffer::const_iterator raw_buffer::end() const
{
    return (buffer_ptr_ + size_);
}

void raw_buffer::clear()
{
    size_ = 0;
    buffer_ptr_ = 0;

    if(buffer_)
    {
        std::free(buffer_);
        buffer_size_ = 0;
        buffer_ = 0;
    }
}

void raw_buffer::grow(std::size_t size)
{
    allocate(size_ + size);
}

void raw_buffer::shrink(std::size_t size)
{
    allocate(size_ > size ? size_ - size : 0);
}

void raw_buffer::resize(std::size_t size)
{
    allocate(size);
}

raw_buffer::value_type& raw_buffer::operator[](std::size_t index)
{
    return buffer_ptr_[index];
}

raw_buffer::value_type const& raw_buffer::operator[](std::size_t index) const
{
    return buffer_ptr_[index];
}

bool raw_buffer::operator<(raw_buffer const& other) const
{
    return std::lexicographical_compare(buffer_ptr_, buffer_ptr_ + size_, other.buffer_ptr_, other.buffer_ptr_ + other.size_);
}

bool raw_buffer::operator==(raw_buffer const& other) const
{
    return (size_ == other.size_ && std::equal(buffer_ptr_, buffer_ptr_ + size_, other.buffer_ptr_));
}

bool raw_buffer::operator!=(raw_buffer const& other) const
{
    return (size_ != other.size_ || !std::equal(buffer_ptr_, buffer_ptr_ + size_, other.buffer_ptr_));
}

void raw_buffer::initialize()
{
    size_ = 0;
    buffer_ptr_ = 0;
    buffer_ = 0;
    buffer_size_ = 0;
}

void raw_buffer::allocate(std::size_t size)
{
    if(size > sizeof(small_buffer_))
    {
        if(size > buffer_size_)
        {
            std::size_t new_size = next_power_of_two(size);
            void* new_buffer = std::realloc(buffer_, new_size);

            if(!new_buffer)
                throw std::bad_alloc();

            buffer_ = reinterpret_cast<value_type*>(new_buffer);

            buffer_size_ = new_size;
            buffer_ptr_ = buffer_;
        }

        if(size_ > 0 && size_ <= sizeof(small_buffer_))
            std::memcpy(buffer_ptr_, small_buffer_, size_);
    }
    else
    {
        buffer_ptr_ = size > 0 ? small_buffer_ : 0;

        if(buffer_)
        {
            if(size > 0 && size_ > sizeof(small_buffer_))
                std::memcpy(buffer_ptr_, buffer_, size);

            std::free(buffer_);
            buffer_size_ = 0;
            buffer_ = 0;
        }
    }

    size_ = size;
}

std::ostream& operator<<(std::ostream& os, raw_buffer const& buffer)
{
    bool has_non_printable = false;

    for(std::size_t i = 0; i < buffer.size(); ++i)
    {
        if(!std::isprint(buffer[i]))
        {
            has_non_printable = true;
            break;
        }
    }

    if(has_non_printable)
    {
        std::ios::fmtflags old_flags = os.flags();

        os.setf(std::ios::uppercase);
        os.unsetf(std::ios::showbase);

        for(std::size_t i = 0; i < buffer.size(); ++i)
        {
            os << "\\x" << std::setw(2) << std::setfill('0');
            os << std::hex << static_cast<int>(buffer[i]);
        }

        os.flags(old_flags);
    }
    else
    {
        for(std::size_t i = 0; i < buffer.size(); ++i)
            os << static_cast<char>(buffer[i]);
    }

    return os;
}

}   // namespace chat
