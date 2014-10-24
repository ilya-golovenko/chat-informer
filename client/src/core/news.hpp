//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_news_news_hpp
#define _chat_core_news_news_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// MISSIO headers
#include <missio/json/json.hpp>

// STL headers
#include <string>


namespace chat
{

class news
{
public:
    news() = default;
    ~news() = default;

    news(news const&) = default;
    news& operator=(news const&) = default;

    news(news const&&) = default;
    news& operator=(news const&&) = default;

    bool update(missio::json::object const& json_data);

    std::wstring const& admin_news() const;
    std::wstring const& moder_news() const;

    std::string to_string() const;
    std::wstring to_wstring() const;

    bool empty() const;
    void clear();

private:
    std::wstring admin_news_;
    std::wstring moder_news_;
};

}   // namespace chat

#endif  // _chat_core_news_news_hpp
