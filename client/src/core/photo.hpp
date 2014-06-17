//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_photos_photo_hpp
#define _missio_core_photos_photo_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <string>


namespace missio
{

class photo
{
public:
    photo(std::wstring const& id,
          std::wstring const& thumb,
          std::wstring const& descr,
          std::wstring const& nickname);

    ~photo();

    photo(photo&& other);
    photo& operator=(photo&& other);

    photo(photo const& other);
    photo& operator=(photo const& other);

    void update(std::wstring const& descr,
                std::wstring const& nickname);

    bool is_deleted() const;
    void set_deleted(bool deleted);

    std::wstring const& id() const;
    std::wstring const& thumb() const;
    std::wstring const& descr() const;
    std::wstring const& nickname() const;

    std::wstring get_photo_uri() const;
    std::wstring get_thumb_uri() const;

private:
    std::wstring id_;
    std::wstring thumb_;
    std::wstring descr_;
    std::wstring nickname_;
    bool is_deleted_;
};

}   // namespace missio

#endif  // _missio_core_photos_photo_hpp
