//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/link_list.hpp>

// STL headers
#include <utility>


namespace chat
{

bool link_list::update(missio::json::object const& json_data)
{
    if(json_data.contains("links"))
    {
        links_.clear();

        for(auto const& json_link : json_data["links"].get_object())
        {
            std::wstring const uri = json_link["uri"];
            std::wstring const name = json_link["name"];
            std::wstring const descr = json_link["descr"];

            links_.emplace_back(uri, name, descr);
        }

        return true;
    }

    return false;
}

void link_list::clear()
{
    links_.clear();
}

bool link_list::empty() const
{
    return links_.empty();
}

std::size_t link_list::size() const
{
    return links_.size();
}

link const& link_list::operator[](std::size_t index) const
{
    return links_[index];
}

}   // namespace chat
