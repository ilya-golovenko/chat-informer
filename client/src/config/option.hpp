//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_config_option_hpp
#define _chat_config_option_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <config/manager.hpp>

// STL headers
#include <string>
#include <vector>


namespace chat
{
namespace config
{

template <typename T>
class option
{
public:
    explicit option(std::string const& path, T const& default_value = T());

    option(option const&) = delete;
    option& operator=(option const&) = delete;

    template <typename U>
    option<T>& operator=(U const& value);

    template <typename U>
    operator U() const;

    bool exists() const;
    void erase();

private:
    path_type path_;
    T default_value_;
};

template <typename T>
class option<std::vector<T>>
{
public:
    explicit option(std::string const& path);

    option(option const&) = delete;
    option& operator=(option const&) = delete;

    template <typename U>
    option<std::vector<T>>& operator=(std::vector<U> const& values);

    template <typename U>
    operator std::vector<U>() const;

    bool exists() const;
    void erase();

private:
    path_type path_;
};

// Implementation headers
#include <config/option.inl>

}   // namespace config
}   // namespace chat

#endif  // _chat_config_option_hpp
