//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <config/manager.hpp>
#include <config/detail/value_helper.hpp>
#include <utility/file_utils.hpp>


namespace chat
{
namespace config
{

manager& manager::instance()
{
    static manager instance;
    return instance;
}

void manager::load_file(boost::filesystem::path const& filename, bool overwrite)
{
    merge_value(missio::json::read(file::read(filename)), overwrite);
}

void manager::save_file(boost::filesystem::path const& filename)
{
    file::write(filename, missio::json::write_formatted(config_));
}

void manager::erase(path_type const& path)
{
    detail::value_helper::erase(config_, path.begin(), path.end());
}

void manager::clear()
{
    config_ = missio::json::null_value;
}

void manager::merge_value(missio::json::value const& value, bool overwrite)
{
    missio::json::object& config = config_.to_object();

    for(auto const& object_value : value.get_object())
    {
        if(overwrite)
            config.erase(object_value.first);

        config.insert(object_value);
    }
}

bool manager::contains_value(path_type const& path) const
{
    return detail::value_helper::contains(config_, path.begin(), path.end());
}

missio::json::value const& manager::get_value(path_type const& path) const
{
    return detail::value_helper::get(config_, path.begin(), path.end());
}

void manager::set_value(path_type const& path, missio::json::value const& value)
{
    detail::value_helper::set(config_, path.begin(), path.end(), value);
}

}   // namespace config
}   // namespace chat
