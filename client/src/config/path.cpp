//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <config/path.hpp>

// BOOST headers
#include <boost/algorithm/string.hpp>

// STL headers
#include <stdexcept>


namespace chat
{
namespace config
{

path_type make_path(std::string const& string)
{
    path_type path;

    boost::algorithm::split(path, string, boost::algorithm::is_any_of("."));

    if(path.empty() || path.size() > 10)
        throw std::runtime_error("invalid config path");

    return path;
}

}   // namespace config
}   // namespace chat
