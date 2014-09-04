//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/forum_post_list.hpp>
#include <core/exception.hpp>

// BOOST headers
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/next_prior.hpp>
#include <boost/bind.hpp>

// STL headers
#include <algorithm>


namespace chat
{

forum_post_list::forum_post_list()
{
}

forum_post_list::~forum_post_list()
{
}

forum_post_list::forum_post_list(forum_post_list&& other) :
    posts_(std::move(other.posts_))
{
}

forum_post_list& forum_post_list::operator=(forum_post_list&& other)
{
    assign(std::forward<forum_post_list>(other));
    return *this;
}

forum_post_list::forum_post_list(forum_post_list const& other) :
    posts_(other.posts_)
{
}

forum_post_list& forum_post_list::operator=(forum_post_list const& other)
{
    assign(other);
    return *this;
}

bool forum_post_list::update(missio::json::object const& json_data)
{
    bool updated = false;

    if(json_data.contains("posts"))
    {
        missio::json::array const& json_posts = json_data["posts"];

        for(missio::json::object const& json_post : json_posts)
        {
            std::wstring const id = json_post["id"];
            std::wstring const title = json_post["title"];
            std::wstring const author = json_post["author"];
            std::time_t const time = json_post["time"];

            if(!contains(id))
            {
                posts_.emplace_back(id, title, author, time);
                updated = true;
            }
        }

        if(updated)
            sort_post_list();
    }

    return updated;
}

void forum_post_list::assign(forum_post_list&& other)
{
    if(&other != this)
        posts_ = std::move(other.posts_);
}

void forum_post_list::assign(forum_post_list const& other)
{
    if(&other != this)
        posts_ = other.posts_;
}

void forum_post_list::clear()
{
    posts_.clear();
}

bool forum_post_list::empty() const
{
    return posts_.empty();
}

std::size_t forum_post_list::size() const
{
    return posts_.size();
}

forum_post_list::iterator forum_post_list::begin()
{
    return posts_.begin();
}

forum_post_list::const_iterator forum_post_list::begin() const
{
    return posts_.begin();
}

forum_post_list::iterator forum_post_list::end()
{
    return posts_.end();
}

forum_post_list::const_iterator forum_post_list::end() const
{
    return posts_.end();
}

forum_post& forum_post_list::front()
{
    return posts_.front();
}

forum_post const& forum_post_list::front() const
{
    return posts_.front();
}

forum_post& forum_post_list::back()
{
    return posts_.back();
}

forum_post const& forum_post_list::back() const
{
    return posts_.back();
}

forum_post_list::iterator forum_post_list::find(std::wstring const& post_id)
{
    return boost::find_if(posts_, boost::bind(&forum_post::id, _1) == post_id);
}

forum_post_list::const_iterator forum_post_list::find(std::wstring const& post_id) const
{
    return boost::find_if(posts_, boost::bind(&forum_post::id, _1) == post_id);
}

bool forum_post_list::contains(std::wstring const& post_id) const
{
    return find(post_id) != end();
}

std::size_t forum_post_list::index_of(std::wstring const& post_id) const
{
    const_iterator it = find(post_id);

    if(it != end())
        return std::distance(begin(), it);

    return invalid_index;
}

forum_post& forum_post_list::operator[](std::size_t index)
{
    return posts_[index];
}

forum_post const& forum_post_list::operator[](std::size_t index) const
{
    return posts_[index];
}

forum_post& forum_post_list::operator[](std::wstring const& post_id)
{
    iterator it = find(post_id);

    if(it == end())
        throw exception(L"cannot find post: " + post_id);

    return *it;
}

forum_post const& forum_post_list::operator[](std::wstring const& post_id) const
{
    const_iterator it = find(post_id);

    if(it == end())
        throw exception(L"cannot find post: " + post_id);

    return *it;
}

void forum_post_list::sort_post_list()
{
    boost::sort(posts_, boost::bind(&forum_post::time, _1) < boost::bind(&forum_post::time, _2));
}

}   // namespace chat
