//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_errors_hpp
#define _missio_core_errors_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <iostream>
#include <string>


namespace missio
{
namespace error
{

enum type
{
    successful,
    network_io_error,
    bad_http_response,
    bad_server_data,
    bad_server_status,
    file_io_error,
    operation_aborted,
    unknown
};

std::string to_string(type error);
std::wstring to_wstring(type error);

inline std::ostream& operator<<(std::ostream& os, type error)
{
    os << to_string(error);
    return os;
}

}   // namespace error
}   // namespace missio

#endif  // _missio_core_errors_hpp
