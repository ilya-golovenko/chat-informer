//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/photo.hpp>
#include <core/common.hpp>

// MISSIO headers
#include <missio/format/format.hpp>

// STL headers
#include <utility>


namespace chat
{

photo::photo(std::wstring const& id,
             std::wstring const& thumb,
             std::wstring const& descr,
             std::wstring const& nickname) :
    id_(id),
    thumb_(thumb),
    descr_(descr),
    nickname_(nickname),
    is_deleted_(false)
{
}

void photo::update(std::wstring const& descr, std::wstring const& nickname)
{
    descr_ = descr_;
    nickname_ = nickname_;
}

bool photo::is_deleted() const
{
    return is_deleted_;
}

void photo::set_deleted(bool deleted)
{
    is_deleted_ = deleted;
}

std::wstring const& photo::id() const
{
    return id_;
}

std::wstring const& photo::thumb() const
{
    return thumb_;
}

std::wstring const& photo::descr() const
{
    return descr_;
}

std::wstring const& photo::nickname() const
{
    return nickname_;
}

std::wstring photo::get_photo_uri() const
{
    std::wstring uri;

    missio::format::print(uri, photoalbum_photo_uri, id_);

    return uri;
}

std::wstring photo::get_thumb_uri() const
{
    std::wstring uri;

    missio::format::print(uri, photoalbum_thumb_uri, thumb_);

    return uri;
}

}   // namespace chat
