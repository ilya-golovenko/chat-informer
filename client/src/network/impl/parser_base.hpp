//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_impl_parser_base_hpp
#define _chat_network_impl_parser_base_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/parse_result.hpp>


namespace chat
{
namespace net
{
namespace impl
{

class parser_base
{
public:
    parser_base() = default;
    ~parser_base() = default;

    parser_base(parser_base const&) = delete;
    parser_base& operator=(parser_base const&) = delete;

public:
    static bool is_char(unsigned char c);
    static bool is_digit(unsigned char c);
    static bool is_xdigit(unsigned char c);
    static bool is_control(unsigned char c);
};

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_impl_parser_base_hpp
