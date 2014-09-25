//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------


template <typename T>
option<T>::option(std::string const& path, T const& default_value) :
    path_(make_path(path)),
    default_value_(default_value)
{
}

template <typename T>
template <typename U>
option<T>& option<T>::operator=(U const& value)
{
    manager::instance().set<T>(path_, T(value));
    return *this;
}

template <typename T>
template <typename U>
option<T>::operator U() const
{
    if(manager::instance().contains<T>(path_))
        return U(manager::instance().get<T>(path_));

    return U(default_value_);
}

template <typename T>
bool option<T>::exists() const
{
    return !manager::instance().contains<T>(path_);
}

template <typename T>
void option<T>::erase()
{
    manager::instance().erase(path_);
}

template <typename T>
option<std::vector<T> >::option(std::string const& path) :
    path_(make_path(path))
{
}

template <typename T>
template <typename U>
option<std::vector<T> >& option<std::vector<T> >::operator=(std::vector<U> const& values)
{
    json::array array;

    for(std::size_t index = 0; index < values.size(); ++index)
    {
        array.push_back(json::value(T(values[index])));
    }

    manager::instance().set<json::array>(path_, array);

    return *this;
}

template <typename T>
template <typename U>
option<std::vector<T> >::operator std::vector<U>() const
{
    std::vector<U> values;

    if(manager::instance().contains<json::array>(path_))
    {
        json::array array = manager::instance().get<json::array>(path_);

        for(std::size_t index = 0; index < array.size(); ++index)
        {
            if(array[index].is<T>())
                values.push_back(U(array[index].as<T>()));
        }
    }

    return values;
}

template <typename T>
bool option<std::vector<T> >::exists() const
{
    if(manager::instance().contains<json::array>(path_))
        return manager::instance().get<json::array>(path_).empty();

    return true;
}

template <typename T>
void option<std::vector<T> >::erase()
{
    manager::instance().erase(path_);
}
