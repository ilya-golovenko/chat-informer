//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_forum_topic_hpp
#define _missio_core_forum_topic_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/forum_post_list.hpp>

// STL headers
#include <vector>
#include <string>


namespace missio
{

class forum_topic
{
public:
    forum_topic(std::wstring const& id,
                std::wstring const& title);

    ~forum_topic();

    forum_topic(forum_topic&& other);
    forum_topic& operator=(forum_topic&& other);

    forum_topic(forum_topic const& other);
    forum_topic& operator=(forum_topic const& other);

    bool update(json::object_cref json_data);

    std::wstring const& id() const;
    std::wstring const& uri() const;
    std::wstring const& title() const;

    void mark_as_old();
    bool is_new() const;

    void mark_as_read();
    bool is_read() const;

    std::time_t time() const;

    forum_post_list const& posts() const;

private:
    std::wstring id_;
    std::wstring title_;

    bool is_new_;

    forum_post_list posts_;
};

}   // namespace missio

#endif  // _missio_core_forum_topic_hpp
