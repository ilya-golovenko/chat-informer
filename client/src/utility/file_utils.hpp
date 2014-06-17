//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _utility_file_utils_hpp
#define _utility_file_utils_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/filesystem.hpp>

// STL headers
#include <string>


namespace util
{
namespace file
{

std::string read(boost::filesystem::path const& filename);

void write(boost::filesystem::path const& filename, std::string const& data);
void append(boost::filesystem::path const& filename, std::string const& data);

}   // namespace file
}   // namespace util

#endif  // _utility_file_utils_hpp
