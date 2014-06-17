//---------------------------------------------------------------------------
//
//    This file is part of BIND library
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _bind_enum_hpp
#define _bind_enum_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application header
#include <utf8/utf8.hpp>
#include <json/json.hpp>


namespace bind
{

struct enum_val
{
    utf8::string name;
    boost::int32_t value;
};

class enum_
{
public:
    enum_();
    ~enum_();

    std::string const& name() const;

    enum_val const& get_value(std::string const& name) const;

private:
    typedef std::vector<enum_val> value_vector;

private:
    std::string name_;
    value_vector values_;
};

}   // namespace bind

#endif  // _bind_enum_hpp
