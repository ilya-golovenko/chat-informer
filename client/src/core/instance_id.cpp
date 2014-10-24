//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/instance_id.hpp>

// BOOST headers
#include <boost/uuid/uuid_io.hpp>


namespace chat
{

instance_id::instance_id(boost::uuids::uuid const& value) :
    value_(value)
{
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
