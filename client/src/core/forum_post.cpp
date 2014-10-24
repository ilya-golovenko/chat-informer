//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/forum_post.hpp>
#include <utility/html_utils.hpp>

// MISSIO headers
#include <missio/format/format.hpp>


namespace chat
{

forum_post::forum_post(std::wstring const& id,
                       std::wstring const& title,
                       std::wstring const& author,
                       std::time_t time) :
    id_(id),
    title_(title),
    author_(author),
    time_(time),
    is_read_(false)
{
    missio::format::print(uri_, forum_post_uri, id_);
}

std::wstring const& forum_post::id() const
{
    return id_;
}

std::wstring const& forum_post::uri() const
{
    return uri_;
}

std::wstring const& forum_post::title() const
{
    return title_;
}

std::wstring const& forum_post::author() const
{
    return author_;
}

std::time_t forum_post::time() const
{
    return time_;
}

void forum_post::mark_as_read()
{
    is_read_ = true;
}

bool forum_post::is_read() const
{
    return is_read_;
}

}   // namespace chat
