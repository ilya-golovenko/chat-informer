//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_impl_response_parser_hpp
#define _chat_network_http_impl_response_parser_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/impl/basic_parser.hpp>
#include <network/http/impl/version_parser.hpp>
#include <network/http/impl/headers_parser.hpp>
#include <network/http/response.hpp>

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

class response_parser : public net::impl::basic_parser<response_parser>
{
public:
    response_parser();

    response_parser(response_parser const&) = delete;
    response_parser& operator=(response_parser const&) = delete;

    void reset();

    parse_result consume(response& response, char c);

private:
    enum
    {
        state_version,
        state_status_code_start,
        state_status_code,
        state_status_reason,
        state_expecting_newline,
        state_headers
    } state_;

    std::size_t size_;
    std::uint16_t status_;

    version_parser version_parser_;
    headers_parser headers_parser_;
};

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_impl_response_parser_hpp
