//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------


template <typename T>
bool manager::contains(path_type const& path) const
{
    return contains_value(path) && get_value(path).is<T>();
}

template <typename T>
T manager::get(path_type const& path) const
{
    return get_value(path).get<T>();
}

template <typename T>
void manager::set(path_type const& path, T const& value)
{
    set_value(path, missio::json::value(value));
}
