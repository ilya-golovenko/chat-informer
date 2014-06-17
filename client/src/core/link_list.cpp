//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/link_list.hpp>

// STL headers
#include <utility>


namespace missio
{

link_list::link_list()
{
}

link_list::~link_list()
{
}

link_list::link_list(link_list&& other) :
    links_(std::move(other.links_))
{
}

link_list& link_list::operator=(link_list&& other)
{
    assign(std::forward<link_list>(other));
    return *this;
}

link_list::link_list(link_list const& other) :
    links_(other.links_)
{
}

link_list& link_list::operator=(link_list const& other)
{
    assign(other);
    return *this;
}

bool link_list::update(json::object_cref json_data)
{
    if(json_data->contains("links"))
    {
        links_.clear();

        json::array_cref json_links = json_data["links"];

        for(std::size_t i = 0; i < json_links->size(); ++i)
        {
            json::object_cref json_link = json_links[i];

            std::wstring uri = json_link["uri"];
            std::wstring name = json_link["name"];
            std::wstring descr = json_link["descr"];

            links_.push_back(link(uri, name, descr));
        }

        return true;
    }

    return false;
}

void link_list::assign(link_list&& other)
{
    if(&other != this)
        links_ = std::move(other.links_);
}

void link_list::assign(link_list const& other)
{
    if(&other != this)
        links_ = other.links_;
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

}   // namespace missio
