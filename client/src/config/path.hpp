//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_config_path_hpp
#define _chat_config_path_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <string>
#include <vector>


namespace chat
{
namespace config
{

typedef std::vector<std::string> path_type;
path_type make_path(std::string const& string);

}   // namespace config
}   // namespace chat

#endif  // _chat_config_path_hpp
