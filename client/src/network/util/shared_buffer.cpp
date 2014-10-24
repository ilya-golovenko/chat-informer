//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/util/shared_buffer.hpp>

// STL headers
#include <cstring>


namespace chat
{
namespace net
{

shared_buffer::shared_buffer() :
    size_(0u)
{
}

shared_buffer::shared_buffer(void const* data, std::size_t size) :
    size_(size),
    data_.reset(new std::uint8_t[size_], std::default_delete<std::uint8_t[]>())
{
    std::memcpy(data_.get(), data, size_);
}

boost::asio::const_buffers_1 shared_buffer::const_buffer() const
{
    return boost::asio::buffer(data_.get(), size_);
}

shared_buffer::operator bool() const
{
    return 0u != size_;
}

shared_buffer make_shared_buffer(char const* data)
{
    return shared_buffer(data, std::strlen(data));
}

shared_buffer make_shared_buffer(std::string const& data)
{
    return shared_buffer(data.data(), data.size());
}

}   // namespace net
}   // namespace chat
