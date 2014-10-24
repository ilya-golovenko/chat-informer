//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_impl_version_parser_hpp
#define _chat_network_http_impl_version_parser_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/impl/basic_parser.hpp>
#include <network/http/message.hpp>

// STL headers
#include <cstdint>
#include <string>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

class version_parser : public net::impl::basic_parser<version_parser>
{
public:
    version_parser();

    version_parser(version_parser const&) = delete;
    version_parser& operator=(version_parser const&) = delete;

    void reset();

    parse_result consume(message& message, char c);

private:
    enum
    {
        state_version_h,
        state_version_t_1,
        state_version_t_2,
        state_version_p,
        state_version_slash,
        state_version_major_start,
        state_version_major,
        state_version_minor_start,
        state_version_minor
    } state_;

    std::uint16_t major_;
    std::uint16_t minor_;
};

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_impl_version_parser_hpp
