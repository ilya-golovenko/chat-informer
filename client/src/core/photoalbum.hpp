//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_photos_photoalbum_hpp
#define _missio_core_photos_photoalbum_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/photo_list.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// BOOST headers
#include <boost/noncopyable.hpp>


namespace missio
{

class photoalbum :
    private boost::noncopyable
{
public:
    photoalbum();
    ~photoalbum();

    photo_list const& fresh_photos() const;

    bool update(json::object_cref json_data);

private:
    photo_list fresh_photos_;
};

}   // namespace missio

#endif  // _missio_core_photos_photoalbum_hpp
