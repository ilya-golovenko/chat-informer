//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_photos_photo_list_hpp
#define _chat_core_photos_photo_list_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/photo.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// STL headers
#include <string>
#include <vector>


namespace chat
{

class photo_list
{
public:
    typedef std::vector<photo>::iterator iterator;
    typedef std::vector<photo>::const_iterator const_iterator;

public:
    photo_list();
    ~photo_list();

    photo_list(photo_list&& other);
    photo_list& operator=(photo_list&& other);

    photo_list(photo_list const& other);
    photo_list& operator=(photo_list const& other);

    bool update(missio::json::object const& json_data);

    bool empty() const;
    std::size_t size() const;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    iterator find(std::wstring const& photo_id);
    const_iterator find(std::wstring const& photo_id) const;

    bool contains(std::wstring const& photo_id) const;
    std::size_t index_of(std::wstring const& photo_id) const;

    photo const& operator[](std::size_t index) const;
    photo const& operator[](std::wstring const& photo_id) const;

public:
    static std::size_t const invalid_index = ~std::size_t(0);

private:
    bool cleanup();
    void sort();

private:
    std::vector<photo> photos_;
};

}   // namespace chat

#endif  // _chat_core_photos_photo_list_hpp
