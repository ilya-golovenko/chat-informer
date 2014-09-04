//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/instance_id.hpp>

// BOOST headers
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


namespace chat
{

instance_id::instance_id() :
    value_(boost::uuids::nil_uuid())
{
}

instance_id::~instance_id()
{
}

instance_id::instance_id(boost::uuids::uuid const& value) :
    value_(value)
{
}

instance_id::instance_id(instance_id const& other) :
    value_(other.value_)
{
}

instance_id& instance_id::operator=(instance_id const& other)
{
    if(&other != this)
        value_ = other.value_;
    return *this;
}

std::string instance_id::to_string() const
{
    return boost::uuids::to_string(value_);
}

std::wstring instance_id::to_wstring() const
{
    return boost::uuids::to_wstring(value_);
}

}   // namespace chat
