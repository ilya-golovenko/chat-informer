//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_forum_topic_list_hpp
#define _missio_core_forum_topic_list_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/forum_topic.hpp>

// STL headers
#include <vector>
#include <string>


namespace missio
{

class forum_topic_list
{
public:
    typedef std::vector<forum_topic>::iterator iterator;
    typedef std::vector<forum_topic>::const_iterator const_iterator;

public:
    forum_topic_list();
    ~forum_topic_list();

    forum_topic_list(forum_topic_list&& other);
    forum_topic_list& operator=(forum_topic_list&& other);

    forum_topic_list(forum_topic_list const& other);
    forum_topic_list& operator=(forum_topic_list const& other);

    bool update(json::object_cref json_data);

    void assign(forum_topic_list&& other);
    void assign(forum_topic_list const& other);

    void mark_as_old();

    void clear();

    bool empty() const;
    std::size_t size() const;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const; 

    forum_topic& front();
    forum_topic const& front() const;

    forum_topic& back();
    forum_topic const& back() const;

    iterator find(std::wstring const& topic_id);
    const_iterator find(std::wstring const& topic_id) const;

    bool contains(std::wstring const& topic_id) const;
    std::size_t index_of(std::wstring const& topic_id) const;

    forum_topic& operator[](std::size_t index);
    forum_topic const& operator[](std::size_t index) const;

    forum_topic& operator[](std::wstring const& topic_id);
    forum_topic const& operator[](std::wstring const& topic_id) const;

public:
    static std::size_t const invalid_index = ~std::size_t(0);

private:
    void sort_topic_list();

private:
    std::vector<forum_topic> topics_;
};

}   // namespace missio

#endif  // _missio_core_forum_topic_list_hpp
