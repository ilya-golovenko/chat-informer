//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_forum_hpp
#define _chat_core_forum_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/forum_topic_list.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// BOOST headers
#include <boost/noncopyable.hpp>


namespace chat
{

class forum :
    private boost::noncopyable
{
public:
    forum();
    ~forum();

    forum_topic_list const& topics() const;

    bool update(missio::json::object const& json_data);

private:
    forum_topic_list topics_;
};

}   // namespace chat

#endif  // _chat_core_forum_hpp
