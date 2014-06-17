//---------------------------------------------------------------------------
//
//    This file is part of BIND library
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _bind_parser_hpp
#define _bind_parser_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application header
#include <json/json.hpp>


namespace bind
{

class parser
{
public:
    parser();
    ~parser();
};

}   // namespace bind

#endif  // _bind_parser_hpp
