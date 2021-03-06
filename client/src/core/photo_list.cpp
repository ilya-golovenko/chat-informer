//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/photo_list.hpp>
#include <core/exception.hpp>
#include <core/common.hpp>

// BOOST headers
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/bind.hpp>

// STL headers
#include <utility>


namespace chat
{

bool photo_list::update(missio::json::object const& json_data)
{
    boost::for_each(photos_, boost::bind(&photo::set_deleted, _1, true));

    if(json_data.contains("photos"))
    {
        for(auto const& json_photo : json_data["photos"].get_object())
        {
            std::wstring const id = json_photo["id"];
            std::wstring const thumb = json_photo["thumb"];

            std::wstring descr; 
            std::wstring nickname;

            if(json_photo.contains("descr"))
                descr = json_photo["descr"].get<std::wstring>();

             if(json_photo.contains("nickname"))
                nickname = json_photo["nickname"].get<std::wstring>();

            iterator it = find(id);

            if(it != end())
            {
                it->update(descr, nickname);
                it->set_deleted(false);
                continue;
            }

            photos_.emplace_back(id, thumb, descr, nickname);
        }

        if(cleanup())
        {
            sort();
            return true;
        }
    }

    return false;
}

bool photo_list::empty() const
{
    return photos_.empty();
}

std::size_t photo_list::size() const
{
    return photos_.size();
}

photo_list::iterator photo_list::begin()
{
    return photos_.begin();
}

photo_list::const_iterator photo_list::begin() const
{
    return photos_.begin();
}

photo_list::iterator photo_list::end()
{
    return photos_.end();
}

photo_list::const_iterator photo_list::end() const
{
    return photos_.end();
}

photo_list::iterator photo_list::find(std::wstring const& photo_id)
{
    return boost::find_if(photos_, boost::bind(&photo::id, _1) == photo_id);
}

photo_list::const_iterator photo_list::find(std::wstring const& photo_id) const
{
    return boost::find_if(photos_, boost::bind(&photo::id, _1) == photo_id);
}

bool photo_list::contains(std::wstring const& photo_id) const
{
    return find(photo_id) != end();
}

std::size_t photo_list::index_of(std::wstring const& photo_id) const
{
    const_iterator it = find(photo_id);

    if(it != end())
        return std::distance(begin(), it);

    return invalid_index;
}

photo const& photo_list::operator[](std::size_t index) const
{
    return photos_[index];
}

photo const& photo_list::operator[](std::wstring const& photo_id) const
{
    const_iterator it = find(photo_id);

    if(it == end())
        throw exception(L"cannot find photo: " + photo_id);

    return *it;
}

bool photo_list::cleanup()
{
    std::size_t size = photos_.size();

    boost::remove_erase_if(photos_, boost::bind(&photo::is_deleted, _1));

    return photos_.size() < size;
}

void photo_list::sort()
{
    boost::sort(photos_, boost::bind(&photo::id, _1) > boost::bind(&photo::id, _2));
}

}   // namespace chat
