//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_utility_html_utils_hpp
#define _chat_utility_html_utils_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <string>


namespace chat
{

std::string escape_html(std::string const& string);
std::string unescape_html(std::string const& string);

}   // namespace chat

#endif  // _chat_utility_html_utils_hpp
