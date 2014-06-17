//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/forum_topic_list.hpp>
#include <core/exception.hpp>

// BOOST headers
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/next_prior.hpp>
#include <boost/bind.hpp>

// STL headers
#include <algorithm>


namespace missio
{

forum_topic_list::forum_topic_list()
{
}

forum_topic_list::~forum_topic_list()
{
}

forum_topic_list::forum_topic_list(forum_topic_list&& other) :
    topics_(std::move(other.topics_))
{
}

forum_topic_list& forum_topic_list::operator=(forum_topic_list&& other)
{
    assign(std::forward<forum_topic_list>(other));
    return *this;
}

forum_topic_list::forum_topic_list(forum_topic_list const& other) :
    topics_(other.topics_)
{
}

forum_topic_list& forum_topic_list::operator=(forum_topic_list const& other)
{
    assign(other);
    return *this;
}

bool forum_topic_list::update(json::object_cref json_data)
{
    bool updated = false;

    if(json_data->contains("topics"))
    {
        json::array_cref json_topics = json_data["topics"];

        for(std::size_t i = 0; i < json_topics->size(); ++i)
        {
            json::object_cref json_topic = json_topics[i];

            std::wstring topic_id = json_topic["id"];
            std::wstring title = json_topic["title"];

            iterator it = find(topic_id);

            if(it == end())
            {
                forum_topic topic(topic_id, title);
                topics_.push_back(std::move(topic));
                it = boost::prior(topics_.end());
            }

            if(it->update(json_topic))
                updated = true;
        }

        if(updated)
            sort_topic_list();
    }

    return updated;
}

void forum_topic_list::assign(forum_topic_list&& other)
{
    if(&other != this)
        topics_ = std::move(other.topics_);
}

void forum_topic_list::assign(forum_topic_list const& other)
{
    if(&other != this)
        topics_ = other.topics_;
}

void forum_topic_list::mark_as_old()
{
    boost::for_each(topics_, boost::bind(&forum_topic::mark_as_old, _1));
}

void forum_topic_list::clear()
{
    topics_.clear();
}

bool forum_topic_list::empty() const
{
    return topics_.empty();
}

std::size_t forum_topic_list::size() const
{
    return topics_.size();
}

forum_topic_list::iterator forum_topic_list::begin()
{
    return topics_.begin();
}

forum_topic_list::const_iterator forum_topic_list::begin() const
{
    return topics_.begin();
}

forum_topic_list::iterator forum_topic_list::end()
{
    return topics_.end();
}

forum_topic_list::const_iterator forum_topic_list::end() const
{
    return topics_.end();
}

forum_topic& forum_topic_list::front()
{
    return topics_.front();
}

forum_topic const& forum_topic_list::front() const
{
    return topics_.front();
}

forum_topic& forum_topic_list::back()
{
    return topics_.back();
}

forum_topic const& forum_topic_list::back() const
{
    return topics_.back();
}

forum_topic_list::iterator forum_topic_list::find(std::wstring const& topic_id)
{
    return boost::find_if(topics_, boost::bind(&forum_topic::id, _1) == topic_id);
}

forum_topic_list::const_iterator forum_topic_list::find(std::wstring const& topic_id) const
{
    return boost::find_if(topics_, boost::bind(&forum_topic::id, _1) == topic_id);
}

bool forum_topic_list::contains(std::wstring const& topic_id) const
{
    return (find(topic_id) != end());
}

std::size_t forum_topic_list::index_of(std::wstring const& topic_id) const
{
    const_iterator it = find(topic_id);

    if(it != end())
        return std::distance(begin(), it);

    return invalid_index;
}

forum_topic& forum_topic_list::operator[](std::size_t index)
{
    return topics_[index];
}

forum_topic const& forum_topic_list::operator[](std::size_t index) const
{
    return topics_[index];
}

forum_topic& forum_topic_list::operator[](std::wstring const& topic_id)
{
    iterator it = find(topic_id);

    if(it == end())
        throw exception(L"cannot find topic: " + topic_id);

    return *it;
}

forum_topic const& forum_topic_list::operator[](std::wstring const& topic_id) const
{
    const_iterator it = find(topic_id);

    if(it == end())
        throw exception(L"cannot find topic: " + topic_id);

    return *it;
}

void forum_topic_list::sort_topic_list()
{
    boost::sort(topics_, boost::bind(&forum_topic::time, _1) < boost::bind(&forum_topic::time, _2));
}

}   // namespace missio
