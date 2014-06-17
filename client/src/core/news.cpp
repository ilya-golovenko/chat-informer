//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/news.hpp>

// MISSIO headers
#include <missio/utf8/convert.hpp>


namespace missio
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

bool news::update(json::object_cref json_data)
{
    if(json_data->contains("news"))
    {
        json::object_cref news = json_data["news"];

        std::wstring admin_news = news["admin"];
        std::wstring moder_news = news["moder"];

        bool updated = (!admin_news_.empty() && !moder_news_.empty()
            && (admin_news_ != admin_news || moder_news_ != moder_news));

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
    return utf8::convert(to_wstring());
}

std::wstring news::to_wstring() const
{
    return (admin_news_ + moder_news_);
}

bool news::empty() const
{
    return (admin_news_.empty() && moder_news_.empty());
}

void news::clear()
{
    admin_news_.clear();
    moder_news_.clear();
}

}   // namespace missio
