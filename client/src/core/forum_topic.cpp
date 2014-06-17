//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/exception.hpp>
#include <core/forum_topic.hpp>
#include <utility/html_utils.hpp>

// BOOST headers
#include <boost/range/algorithm.hpp>
#include <boost/bind.hpp>


namespace missio
{

forum_topic::forum_topic(std::wstring const& id,
                         std::wstring const& title) :
    id_(id),
    title_(title),
    is_new_(true)
{
}

forum_topic::~forum_topic()
{
}

forum_topic::forum_topic(forum_topic&& other) :
    id_(std::move(other.id_)),
    title_(std::move(other.title_)),
    is_new_(std::move(other.is_new_)),
    posts_(std::move(other.posts_))
{
}

forum_topic& forum_topic::operator=(forum_topic&& other)
{
    if(&other != this)
    {
        id_ = std::move(other.id_);
        title_ = std::move(other.title_);
        is_new_ = std::move(other.is_new_);
        posts_ = std::move(other.posts_);
    }

    return *this;
}

forum_topic::forum_topic(forum_topic const& other) :
    id_(other.id_),
    title_(other.title_),
    is_new_(other.is_new_),
    posts_(other.posts_)
{
}

forum_topic& forum_topic::operator=(forum_topic const& other)
{
    if(&other != this)
    {
        id_ = other.id_;
        title_ = other.title_;
        is_new_ = other.is_new_;
        posts_ = other.posts_;
    }

    return *this;
}

bool forum_topic::update(json::object_cref json_data)
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
    boost::for_each(posts_, boost::bind(&forum_post::mark_as_read, _1));
}

bool forum_topic::is_read() const
{
    return (boost::find_if(posts_, !boost::bind(&forum_post::is_read, _1)) == posts_.end());
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

}    // namespace missio
