//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_photos_photoalbum_hpp
#define _chat_core_photos_photoalbum_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/photo_list.hpp>

// MISSIO headers
#include <missio/json/json.hpp>


namespace chat
{

class photoalbum
{
public:
    photoalbum() = default;
    ~photoalbum() = default;

    photoalbum(photoalbum const&) = delete;
    photoalbum& operator=(photoalbum const&) = delete;

    photo_list const& fresh_photos() const;

    bool update(missio::json::object const& json_data);

private:
    photo_list fresh_photos_;
};

}   // namespace chat

#endif  // _chat_core_photos_photoalbum_hpp
