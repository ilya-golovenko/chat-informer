//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/news.hpp>

// MISSIO headers
#include <missio/unicode/convert.hpp>


namespace chat
{

news::news()
{
}

news::~news()
{
}

news::news(news const& other) :
    admin_news_(other.admin_news_),
    moder_news_(other.moder_news_)
{
}

news& news::operator=(news const& other)
{
    assign(other);
    return *this;
}

void news::assign(news const& other)
{
    if(&other != this)
    {
        admin_news_ = other.admin_news_;
        moder_news_ = other.moder_news_;
    }
}

bool news::update(missio::json::object const& json_data)
{
    if(json_data.contains("news"))
    {
        missio::json::object const& news = json_data["news"];

        std::wstring const admin_news = news["admin"];
        std::wstring const moder_news = news["moder"];

        bool const updated = !admin_news_.empty() && !moder_news_.empty()
            && (admin_news_ != admin_news || moder_news_ != moder_news);

        admin_news_ = admin_news;
        moder_news_ = moder_news;

        return updated;
    }

    return false;
}

std::wstring const& news::admin_news() const
{
    return admin_news_;
}

std::wstring const& news::moder_news() const
{
    return moder_news_;
}

std::string news::to_string() const
{
    return missio::unicode::to_utf8_string(to_wstring());
}

std::wstring news::to_wstring() const
{
    return admin_news_ + moder_news_;
}

bool news::empty() const
{
    return admin_news_.empty() && moder_news_.empty();
}

void news::clear()
{
    admin_news_.clear();
    moder_news_.clear();
}

}   // namespace chat
