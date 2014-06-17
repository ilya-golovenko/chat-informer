//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_news_news_hpp
#define _missio_core_news_news_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// MISSIO headers
#include <missio/json/json.hpp>

// STL headers
#include <string>


namespace missio
{

class news
{
public:
    news();
    ~news();

    news(news const& other);
    news& operator=(news const& other);

    void assign(news const& other);

    bool update(json::object_cref json_data);

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

}   // namespace missio

#endif  // _missio_core_news_news_hpp
