//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/impl/parser_base.hpp>


namespace chat
{
namespace net
{
namespace impl
{

static char const char_table[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0
};

bool parser_base::is_char(unsigned char c)
{
    return c <= 127u && !!char_table[c];
}

bool parser_base::is_digit(unsigned char c)
{
    return c >= '0' && c <= '9';
}

bool parser_base::is_xdigit(unsigned char c)
{
    return c >= '0' && c <= '9' ||
           c >= 'a' && c <= 'f' ||
           c >= 'A' && c <= 'F';
}

bool parser_base::is_control(unsigned char c)
{
    return c <= 31u || c == 127u;
}

}   // namespace impl
}   // namespace net
}   // namespace chat
