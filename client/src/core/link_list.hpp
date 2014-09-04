//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_links_link_list_hpp
#define _chat_core_links_link_list_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/link.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// STL headers
#include <vector>


namespace chat
{

class link_list
{

public:
    link_list();
    ~link_list();

    link_list(link_list&& other);
    link_list& operator=(link_list&& other);

    link_list(link_list const& other);
    link_list& operator=(link_list const& other);

    bool update(missio::json::object const& json_data);

    void assign(link_list&& other);
    void assign(link_list const& other);

    void clear();

    bool empty() const;
    std::size_t size() const;

    link const& operator[](std::size_t index) const;

private:
    std::vector<link> links_;
};

}   // namespace chat

#endif  // _chat_core_links_link_list_hpp
