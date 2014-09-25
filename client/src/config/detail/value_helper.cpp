//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <config/detail/value_helper.hpp>


namespace chat
{
namespace config
{
namespace detail
{

void value_helper::set(missio::json::value& parent,
                       path_type::const_iterator pos,
                       path_type::const_iterator end,
                       missio::json::value const& value)
{
    if(pos == end)
    {
        parent = value;
        return;
    }

    set(parent[*pos], pos + 1, end, value);
}

void value_helper::erase(missio::json::value& parent,
                         path_type::const_iterator pos,
                         path_type::const_iterator end)
{
    if(pos == end)
    {
        parent.to_object().erase(*pos);
        return;
    }

    erase(parent[*pos], pos + 1, end);
}

bool value_helper::contains(missio::json::value const& parent,
                            path_type::const_iterator pos,
                            path_type::const_iterator end)
{
    if(pos == end)
        return true;

    if(parent.is_object())
    {
        if(parent.get_object().contains(*pos))
            return contains(parent[*pos], pos + 1, end);
    }

    return false;
}

missio::json::value const& value_helper::get(missio::json::value const& parent,
                                             path_type::const_iterator pos,
                                             path_type::const_iterator end)
{
    if(pos == end)
        return parent;

    return get(parent[*pos], pos + 1, end);
}

}   // namespace detail
}   // namespace config
}   // namespace chat
