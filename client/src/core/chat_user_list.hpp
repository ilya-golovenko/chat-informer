//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_chat_user_list_hpp
#define _missio_core_chat_user_list_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/chat_user.hpp>

// STL headers
#include <vector>
#include <string>


namespace missio
{

class chat_user_list
{
public:
    typedef std::vector<chat_user> vector_type;
    typedef std::vector<chat_user>::iterator iterator;
    typedef std::vector<chat_user>::const_iterator const_iterator;

public:
    chat_user_list();
    ~chat_user_list();

    explicit chat_user_list(std::vector<chat_user>&& users);

    chat_user_list(chat_user_list&& other);
    chat_user_list& operator=(chat_user_list&& other);

    chat_user_list(chat_user_list const& other);
    chat_user_list& operator=(chat_user_list const& other);

    void assign(chat_user_list&& other);
    void assign(chat_user_list const& other);

    void sort(bool sort_by_nicknames = true);

    void push_back(chat_user&& user);
    void push_back(chat_user const& user);

    void erase(iterator where);
    void erase(iterator first, iterator last);

    void erase(std::wstring const& nickname);

    void clear();

    bool empty() const;
    std::size_t size() const;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    iterator find(std::wstring const& nickname);
    const_iterator find(std::wstring const& nickname) const;

    bool contains(std::wstring const& nickname) const;
    std::size_t index_of(std::wstring const& nickname) const;

    chat_user& operator[](std::size_t index);
    chat_user const& operator[](std::size_t index) const;

    chat_user& operator[](std::wstring const& nickname);
    chat_user const& operator[](std::wstring const& nickname) const;

public:
    static std::size_t const invalid_index = ~std::size_t(0);

private:
    static bool are_equal(chat_user const& user, std::wstring const& nickname);
    static bool compare(chat_user const& left, chat_user const& right, bool by_nickname);

private:
    std::vector<chat_user> users_;
};

}   // namespace missio

#endif  // _missio_core_chat_user_list_hpp
