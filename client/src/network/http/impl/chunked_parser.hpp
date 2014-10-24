//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_impl_chunked_parser_hpp
#define _chat_network_http_impl_chunked_parser_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/impl/basic_parser.hpp>
#include <network/http/message.hpp>

// STL headers
#include <cstddef>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

class chunked_parser : public net::impl::basic_parser<chunked_parser>
{
public:
    chunked_parser();

    chunked_parser(chunked_parser const&) = delete;
    chunked_parser& operator=(chunked_parser const&) = delete;

    void reset();

    parse_result consume(char c);

    std::size_t get_chunk_size() const;

private:
    static int hex_to_int(int c);

private:
    enum
    {
        state_chunk_size_start,
        state_chunk_size,
        state_expecting_newline_1,
        state_expecting_newline_2,
        state_expecting_newline_3
    } state_;

    std::size_t size_;
    std::size_t chunk_size_;
};

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_impl_chunked_parser_hpp
