//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_forum_post_hpp
#define _chat_core_forum_post_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <string>
#include <ctime>


namespace chat
{

class forum_post
{
public:
    forum_post(std::wstring const& id,
               std::wstring const& title,
               std::wstring const& author,
               std::time_t time);

    ~forum_post();

    forum_post(forum_post&& other);
    forum_post& operator=(forum_post&& other);

    forum_post(forum_post const& other);
    forum_post& operator=(forum_post const& other);

    std::wstring const& id() const;
    std::wstring const& uri() const;
    std::wstring const& title() const;
    std::wstring const& author() const;

    std::time_t time() const;

    void mark_as_read();
    bool is_read() const;

private:
    std::wstring id_;
    std::wstring uri_;
    std::wstring title_;
    std::wstring author_;
    std::time_t time_;
    bool is_read_;
};

}   // namespace chat

#endif  // _chat_core_forum_post_hpp
