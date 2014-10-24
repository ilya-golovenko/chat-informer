//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/credentials.hpp>

// STL headers
#include <sstream>


namespace chat
{
namespace net
{
namespace http
{

credentials::credentials(std::string const& username, std::string const& password)
{
    set_username(username);
    set_password(password);
}

void credentials::set_username(std::string const& username)
{
    std::size_t const pos = username.find_first_of('\\');

    if(std::string::npos != pos)
    {
        username_ = username.substr(0, pos);
        domain_ = username.substr(pos + 1);
    }
    else
    {
        username_ = username;
        domain_.clear();
    }
}

void credentials::set_password(std::string const& password)
{
    password_ = password;
}

std::string const& credentials::username() const
{
    return username_;
}

std::string const& credentials::password() const
{
    return password_;
}

std::string const& credentials::domain() const
{
    return domain_;
}

bool credentials::empty() const
{
    return username_.empty();
}

void credentials::clear()
{
    username_.clear();
    password_.clear();
    domain_.clear();
}

std::string credentials::to_string() const
{
    std::ostringstream str;

    if(!username_.empty())
    {
        if(!domain_.empty())
            str << domain_ << '\\';

        str << username_;

        if(!password_.empty())
            str << ':' << password_;
    }

    return str.str();
}

}   // namespace http
}   // namespace net
}   // namespace chat
