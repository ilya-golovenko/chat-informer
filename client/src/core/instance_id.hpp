//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_instance_id_hpp
#define _missio_core_instance_id_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// BOOST headers
#include <boost/uuid/uuid.hpp>

// STL headers
#include <iostream>
#include <string>


namespace missio
{

class instance_id
{
public:
    instance_id();
    ~instance_id();

    explicit instance_id(boost::uuids::uuid const& value);

    instance_id(instance_id const& other);
    instance_id& operator=(instance_id const& other);

    std::string to_string() const;
    std::wstring to_wstring() const;

private:
    boost::uuids::uuid value_;
};

inline std::ostream& operator<<(std::ostream& os, instance_id const& instance_id)
{
    os << instance_id.to_string();
    return os;
}

}   // namespace missio

#endif  // _missio_core_instance_id_hpp
