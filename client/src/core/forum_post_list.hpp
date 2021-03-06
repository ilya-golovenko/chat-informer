//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_forum_post_list_hpp
#define _chat_core_forum_post_list_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/forum_post.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// STL headers
#include <vector>
#include <string>


namespace chat
{

class forum_post_list
{
public:
    typedef std::vector<forum_post>::iterator iterator;
    typedef std::vector<forum_post>::const_iterator const_iterator;

public:
    forum_post_list() = default;
    ~forum_post_list() = default;

    forum_post_list(forum_post_list const&) = default;
    forum_post_list& operator=(forum_post_list const&) = default;

    forum_post_list(forum_post_list&&) = default;
    forum_post_list& operator=(forum_post_list&&) = default;

    bool update(missio::json::object const& json_data);

    void clear();

    bool empty() const;
    std::size_t size() const;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const; 

    forum_post& front();
    forum_post const& front() const;

    forum_post& back();
    forum_post const& back() const;

    iterator find(std::wstring const& post_id);
    const_iterator find(std::wstring const& post_id) const;

    bool contains(std::wstring const& post_id) const;
    std::size_t index_of(std::wstring const& post_id) const;

    forum_post& operator[](std::size_t index);
    forum_post const& operator[](std::size_t index) const;

    forum_post& operator[](std::wstring const& post_id);
    forum_post const& operator[](std::wstring const& post_id) const;

public:
    static std::size_t const invalid_index = ~std::size_t(0);

private:
    void sort_post_list();

private:
    std::vector<forum_post> posts_;
};

}   // namespace chat

#endif  // _chat_core_forum_post_list_hpp
