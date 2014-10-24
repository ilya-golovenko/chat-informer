//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/header.hpp>

// BOOST headers
#include <boost/algorithm/string.hpp>


namespace chat
{
namespace net
{
namespace http
{

header::header(std::string const& name, std::string const& value) :
    name_(name),
    value_(value)
{
}

std::string const& header::get_name() const
{
    return name_;
}

void header::set_name(std::string const& name)
{
    name_ = name;
}

std::string const& header::get_value() const
{
    return value_;
}

void header::set_value(std::string const& value)
{
    value_ = value;
}

void header::serialize(std::ostream& os) const
{
    os << name_ << ": " << value_;
}

bool operator==(header const& lhs, header const& rhs)
{
    return boost::algorithm::iequals(lhs.name_, rhs.name_);
}

bool operator<(header const& lhs, header const& rhs)
{
    return boost::algorithm::ilexicographical_compare(lhs.name_, rhs.name_);
}

}   // namespace http
}   // namespace net
}   // namespace chat
