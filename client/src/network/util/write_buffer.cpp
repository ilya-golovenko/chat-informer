//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/util/write_buffer.hpp>


namespace chat
{
namespace net
{

write_buffer::write_buffer(unique_buffer&& buffer) :
    unique_buffer_(std::move(buffer))
{
}

write_buffer::write_buffer(shared_buffer const& buffer) :
    shared_buffer_(buffer)
{
}

boost::asio::const_buffers_1 write_buffer::const_buffer() const
{
    return unique_buffer_ ? unique_buffer_.const_buffer()
                          : shared_buffer_.const_buffer();
}

}   // namespace net
}   // namespace chat
