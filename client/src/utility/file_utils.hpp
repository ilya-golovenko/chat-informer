//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_utility_file_utils_hpp
#define _chat_utility_file_utils_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/filesystem/path.hpp>

// STL headers
#include <string>


namespace chat
{
namespace file
{

std::string read(boost::filesystem::path const& filename);

void write(boost::filesystem::path const& filename, std::string const& data);
void append(boost::filesystem::path const& filename, std::string const& data);

}   // namespace file
}   // namespace chat

#endif  // _chat_utility_file_utils_hpp
