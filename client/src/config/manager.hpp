//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_config_manager_hpp
#define _chat_config_manager_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <config/path.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// BOOST headers
#include <boost/filesystem.hpp>


namespace chat
{
namespace config
{

class manager
{
public:
    static manager& instance();

public:
    manager() = default;
    ~manager() = default;

    manager(manager const&) = delete;
    manager& operator=(manager const&) = delete;

    void load_file(boost::filesystem::path const& filename, bool overwrite);
    void save_file(boost::filesystem::path const& filename);

    template <typename T>
    bool contains(path_type const& path) const;

    template <typename T>
    T get(path_type const& path) const;

    template <typename T>
    void set(path_type const& path, T const& value);

    void erase(path_type const& path);
    void clear();

private:
    void merge_value(missio::json::value const& value, bool overwrite);

    bool contains_value(path_type const& path) const;
    missio::json::value const& get_value(path_type const& path) const;
    void set_value(path_type const& path, missio::json::value const& value);

private:
    missio::json::value config_;
};

// Implementation headers
#include <config/manager.inl>

}   // namespace config
}   // namespace chat

#endif  // _chat_config_manager_hpp
