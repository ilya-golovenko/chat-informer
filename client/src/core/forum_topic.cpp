//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/exception.hpp>
#include <core/forum_topic.hpp>
#include <utility/html_utils.hpp>

// BOOST headers
#include <boost/range/algorithm.hpp>


namespace chat
{

forum_topic::forum_topic(std::wstring const& id, std::wstring const& title) :
    id_(id),
    title_(title),
    is_new_(true)
{
}

bool forum_topic::update(missio::json::object const& json_data)
{
    return posts_.update(json_data);
}

std::wstring const& forum_topic::id() const
{
    return id_;
}

std::wstring const& forum_topic::uri() const
{
    if(!posts_.empty())
        return posts_.front().uri();

    throw exception("forum topic does not contain posts");
}

std::wstring const& forum_topic::title() const
{
    return title_;
}

void forum_topic::mark_as_old()
{
    is_new_ = false;
}

bool forum_topic::is_new() const
{
    return is_new_;
}

void forum_topic::mark_as_read()
{
    for(auto& post : posts_)
    {
        post.mark_as_read();
    }
}

bool forum_topic::is_read() const
{
    for(auto const& post : posts_)
    {
        if(!post.is_read())
        {
            return false;
        }
    }

    return true;
}

std::time_t forum_topic::time() const
{
    if(!posts_.empty())
        return posts_.back().time();

    throw exception("forum topic does not contain posts");
}

forum_post_list const& forum_topic::posts() const
{
    return posts_;
}

}    // namespace chat
