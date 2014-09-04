//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_utility_make_hex_string_hpp
#define _chat_utility_make_hex_string_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <cstddef>
#include <string>


namespace chat
{

std::string make_hex_string(unsigned char const* input, std::size_t size);

}   // namespace chat

#endif  // _chat_utility_make_hex_string_hpp
