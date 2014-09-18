//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <utility/make_hex_string.hpp>


namespace chat
{

static char const hex_digits[] = "0123456789ABCDEF";

std::string make_hex_string(unsigned char const* input, std::size_t size)
{
    std::string output;

    output.reserve(size * 2);

    for(std::size_t i = 0; i < size; ++i)
    {
        output.push_back(hex_digits[input[i] >> 4]);
        output.push_back(hex_digits[input[i] & 15]);
    }

    return output;
}

}   // namespace chat
