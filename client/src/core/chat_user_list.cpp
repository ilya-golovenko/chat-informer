//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/chat_user_list.hpp>
#include <core/exception.hpp>

// BOOST headers
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/bind.hpp>


namespace chat
{

chat_user_list::chat_user_list(std::vector<chat_user>&& users) :
    users_(std::move(users))
{
}

void chat_user_list::sort(bool sort_by_nicknames)
{
    boost::sort(users_, boost::bind(&compare, _1, _2, sort_by_nicknames));
}

void chat_user_list::push_back(chat_user&& user)
{
    if(!contains(user.nickname()))
        users_.push_back(std::forward<chat_user>(user));
}

void chat_user_list::push_back(chat_user const& user)
{
    if(!contains(user.nickname()))
        users_.push_back(user);
}

void chat_user_list::erase(iterator where)
{
    users_.erase(where);
}

void chat_user_list::erase(iterator first, iterator last)
{
    users_.erase(first, last);
}

void chat_user_list::erase(std::wstring const& nickname)
{
    boost::remove_erase_if(users_, boost::bind(&are_equal, _1, nickname));
}

void chat_user_list::clear()
{
    users_.clear();
}

bool chat_user_list::empty() const
{
    return users_.empty();
}

std::size_t chat_user_list::size() const
{
    return users_.size();
}

chat_user_list::iterator chat_user_list::begin()
{
    return users_.begin();
}

chat_user_list::const_iterator chat_user_list::begin() const
{
    return users_.begin();
}
    
chat_user_list::iterator chat_user_list::end()
{
    return users_.end();
}

chat_user_list::const_iterator chat_user_list::end() const
{
    return users_.end();
}

chat_user_list::iterator chat_user_list::find(std::wstring const& nickname)
{
    return boost::find_if(users_, boost::bind(&are_equal, _1, nickname));
}

chat_user_list::const_iterator chat_user_list::find(std::wstring const& nickname) const
{
    return boost::find_if(users_, boost::bind(&are_equal, _1, nickname));
}

chat_user& chat_user_list::operator[](std::size_t index)
{
    return users_[index];
}

chat_user const& chat_user_list::operator[](std::size_t index) const
{
    return users_[index];
}

chat_user& chat_user_list::operator[](std::wstring const& nickname)
{
    iterator it = find(nickname);

    if(it == end())
        throw exception(L"cannot find user: " + nickname);

    return *it;
}

chat_user const& chat_user_list::operator[](std::wstring const& nickname) const
{
    const_iterator it = find(nickname);

    if(it == end())
        throw exception(L"cannot find user: " + nickname);

    return *it;
}

bool chat_user_list::contains(std::wstring const& nickname) const
{
    return find(nickname) != end();
}

std::size_t chat_user_list::index_of(std::wstring const& nickname) const
{
    const_iterator it = find(nickname);

    if(it != end())
        return std::distance(begin(), it);

    return invalid_index;
}

bool chat_user_list::are_equal(chat_user const& user, std::wstring const& nickname)
{
    return boost::algorithm::iequals(user.nickname(), nickname);
}

bool chat_user_list::compare(chat_user const& left, chat_user const& right, bool by_nickname)
{
    if(by_nickname)
        return boost::algorithm::ilexicographical_compare(left.nickname(), right.nickname());

    return left.sex() != right.sex() ? left.sex() > right.sex() : compare(left, right, true);
}

}   // namespace chat
