//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_impl_headers_parser_hpp
#define _chat_network_http_impl_headers_parser_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/impl/basic_parser.hpp>
#include <network/http/message.hpp>

// STL headers
#include <string>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

class headers_parser : public net::impl::basic_parser<headers_parser>
{
public:
    headers_parser();

    headers_parser(headers_parser const&) = delete;
    headers_parser& operator=(headers_parser const&) = delete;

    void reset();

    parse_result consume(message& message, char c);

private:
    enum
    {
        state_header_line_start,
        state_header_whitespace,
        state_header_name,
        state_space_before_header_value,
        state_header_value_start,
        state_header_value,
        state_expecting_newline_1,
        state_expecting_newline_2
    } state_;

    std::string name_;
    std::string value_;
};

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_impl_headers_parser_hpp
