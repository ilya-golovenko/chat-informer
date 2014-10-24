//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/photoalbum.hpp>


namespace chat
{

photo_list const& photoalbum::fresh_photos() const
{
    return fresh_photos_;
}

bool photoalbum::update(missio::json::object const& json_data)
{
    if(json_data.contains("photoalbum"))
    {
        return fresh_photos_.update(json_data["photoalbum"]);
    }

    return false;
}

}   // namespace chat
