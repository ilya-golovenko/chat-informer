//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
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


namespace chat
{

bool forum_topic_list::update(missio::json::object const& json_data)
{
    bool updated = false;

    if(json_data.contains("topics"))
    {
        for(auto const& json_topic : json_data["topics"].get_object())
        {
            std::wstring const id = json_topic["id"];
            std::wstring const title = json_topic["title"];

            iterator it = find(id);

            if(it == end())
            {
                topics_.emplace_back(id, title);
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

void forum_topic_list::mark_as_old()
{
    for(auto& topic : topics_)
    {
        topic.mark_as_old();
    }
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
    return find(topic_id) != end();
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

}   // namespace chat
