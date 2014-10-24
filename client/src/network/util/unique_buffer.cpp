//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/util/unique_buffer.hpp>

// STL headers
#include <cstring>


namespace chat
{
namespace net
{

unique_buffer::unique_buffer() :
    size_(0u)
{
}

unique_buffer::unique_buffer(void const* data, std::size_t size) :
    size_(size),
    data_.reset(new std::uint8_t[size_])
{
    std::memcpy(data_.get(), data, size_);
}

boost::asio::const_buffers_1 unique_buffer::const_buffer() const
{
    return boost::asio::buffer(data_.get(), size_);
}

unique_buffer::operator bool() const
{
    return 0u != size_;
}

unique_buffer make_unique_buffer(char const* data)
{
    return unique_buffer(data, std::strlen(data));
}

unique_buffer make_unique_buffer(std::string const& data)
{
    return unique_buffer(data.data(), data.size());
}

}   // namespace net
}   // namespace chat
