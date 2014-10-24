//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_impl_request_parser_hpp
#define _chat_network_http_impl_request_parser_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/impl/basic_parser.hpp>
#include <network/http/impl/version_parser.hpp>
#include <network/http/impl/headers_parser.hpp>
#include <network/http/request.hpp>

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

class request_parser : public net::impl::basic_parser<request_parser>
{
public:
    request_parser();

    request_parser(request_parser const&) = delete;
    request_parser& operator=(request_parser const&) = delete;

    void reset();

    parse_result consume(request& request, char c);

private:
    enum
    {
        state_method_start,
        state_method,
        state_url_start,
        state_url,
        state_http_version,
        state_expecting_newline,
        state_headers
    } state_;

    std::size_t size_;
    std::string method_;
    std::string url_;

    version_parser version_parser_;
    headers_parser headers_parser_;
};

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_impl_request_parser_hpp
