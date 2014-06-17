//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_link_hpp
#define _missio_core_link_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <string>


namespace missio
{

class link
{
public:
    link(std::wstring const& uri,
         std::wstring const& name,
         std::wstring const& descr);

    ~link();

    link(link&& other);
    link& operator=(link&& other);

    link(link const& other);
    link& operator=(link const& other);

    std::wstring const& uri() const;
    std::wstring const& name() const;
    std::wstring const& descr() const;

private:
    std::wstring uri_;
    std::wstring name_;
    std::wstring descr_;
};

}   // namespace missio

#endif  // _missio_core_link_hpp
