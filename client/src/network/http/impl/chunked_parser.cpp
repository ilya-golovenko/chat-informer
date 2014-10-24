//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/impl/chunked_parser.hpp>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

chunked_parser::chunked_parser() :
    state_(state_chunk_size_start),
    size_(0u),
    chunk_size_(0u)
{
}

void chunked_parser::reset()
{
    state_= state_chunk_size_start;
    size_ = 0u;
    chunk_size_ = 0u;
}

parse_result chunked_parser::consume(char c)
{
    if(++size_ > max_chunk_size_length)
        return parse_result::failure;

    switch(state_)
    {
        case state_chunk_size_start:
            if(is_xdigit(c))
            {
                chunk_size_ = hex_to_int(c);
                state_ = state_chunk_size;
                return parse_result::more;
            }
            else if(c == '\r' || c == '\n')
            {
                return parse_result::more;
            }
            break;

        case state_chunk_size:
            if(is_xdigit(c))
            {
                chunk_size_ = chunk_size_ << 4;
                chunk_size_ += hex_to_int(c);
                state_ = state_chunk_size;
                return parse_result::more;
            }
            else if(c == '\r')
            {
                state_ = state_expecting_newline_1;
                return parse_result::more;
            }
            break;

        case state_expecting_newline_1:
            if(c == '\n')
            {
                if(chunk_size_ > 0u)
                {
                    return parse_result::ok;
                }
                else
                {
                    state_ = state_expecting_newline_2;
                    return parse_result::more;
                }
            }
            break;

        case state_expecting_newline_2:
            if(c == '\r')
            {
                state_ = state_expecting_newline_3;
                return parse_result::more;
            }
            break;

        case state_expecting_newline_3:
            if(c == '\n')
            {
                return parse_result::ok;
            }
            break;
    }

    return parse_result::failure;
}

std::size_t chunked_parser::get_chunk_size() const
{
    return chunk_size_;
}

int chunked_parser::hex_to_int(int c)
{
    if(c >= '0' && c <= '9')
        return c - '0';

    if(c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 0;
} 

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat
