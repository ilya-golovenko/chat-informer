//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 201, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_config_detail_value_helper_hpp
#define _chat_config_detail_value_helper_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <config/path.hpp>

// MISSIO headers
#include <missio/json/json.hpp>


namespace chat
{
namespace config
{
namespace detail
{

struct value_helper
{
    value_helper() = delete;
    ~value_helper() = delete;

    static void set(missio::json::value& parent,
                    path_type::const_iterator pos,
                    path_type::const_iterator end,
                    missio::json::value const& value);

    static void erase(missio::json::value& parent,
                      path_type::const_iterator pos,
                      path_type::const_iterator end);

    static bool contains(missio::json::value const& parent,
                         path_type::const_iterator pos,
                         path_type::const_iterator end);

    static missio::json::value const& get(missio::json::value const& parent,
                                          path_type::const_iterator pos,
                                          path_type::const_iterator end);
};

}   // namespace detail
}   // namespace config
}   // namespace chat

#endif  // _chat_config_detail_value_helper_hpp
