//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _utility_html_utils_hpp
#define _utility_html_utils_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <string>


namespace util
{

std::string escape_html(std::string const& string);
std::string unescape_html(std::string const& string);

}   // namespace util

#endif  // _utility_html_utils_hpp
