//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/photoalbum.hpp>


namespace missio
{

photoalbum::photoalbum()
{
}

photoalbum::~photoalbum()
{
}

photo_list const& photoalbum::fresh_photos() const
{
    return fresh_photos_;
}

bool photoalbum::update(json::object_cref json_data)
{
    if(json_data->contains("photoalbum"))
        return fresh_photos_.update(json_data["photoalbum"]);

    return false;
}

}   // namespace missio
