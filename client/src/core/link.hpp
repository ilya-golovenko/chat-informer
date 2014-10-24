//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_link_hpp
#define _chat_core_link_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <string>


namespace chat
{

class link
{
public:
    link(std::wstring const& uri,
         std::wstring const& name,
         std::wstring const& descr);

    link(link const&) = default;
    link& operator=(link const&) = default;

    link(link&&) = default;
    link& operator=(link&&) = default;

    std::wstring const& uri() const;
    std::wstring const& name() const;
    std::wstring const& descr() const;

private:
    std::wstring uri_;
    std::wstring name_;
    std::wstring descr_;
};

}   // namespace chat

#endif  // _chat_core_link_hpp
