//---------------------------------------------------------------------------
//
//    This file is part of BIND library
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _bind_struct_hpp
#define _bind_struct_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application header
#include <utf8/utf8.hpp>
#include <json/json.hpp>


namespace bind
{

enum value_attr
{
    required,
    optional,
    repeated
};

enum value_type
{
    string,
    int32,
    uint32,
    int64,
    uint64,
    bool_,
    double_,
    enum_,      // requires type name
    struct_     // requires type name
};

struct struct_val
{
    value_attr value_attr;
    value_type value_type;
    utf8::string type_name;
    utf8::string value_name;
    utf8::string json_name;
};

class struct_
{
public:
    struct_();
    ~struct_();

    std::string const& name() const;

    struct_val const& get_value(std::string const& name) const;

private:
    typedef std::vector<struct_val> value_vector;

private:
    std::string name_;
    value_vector values_;
};

}   // namespace bind

#endif  // _bind_struct_hpp
