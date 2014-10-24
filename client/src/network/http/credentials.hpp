//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _net_http_credentials_hpp
#define _net_http_credentials_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <iostream>
#include <string>


namespace chat
{
namespace net
{
namespace http
{

class credentials
{
public:
    credentials() = default;
    ~credentials() = default;

    credentials(std::string const& username, std::string const& password);

    credentials(credentials const&) = default;
    credentials& operator=(credentials const&) = default;

    credentials(credentials&&) = default;
    credentials& operator=(credentials&&) = default;

    void set_username(std::string const& username);
    void set_password(std::string const& password);

    std::string const& username() const;
    std::string const& password() const;
    std::string const& domain() const;

    bool empty() const;
    void clear();

    std::string to_string() const;

private:
    std::string username_;
    std::string password_;
    std::string domain_;
};

inline std::ostream& operator<<(std::ostream& os, credentials const& credentials)
{
    os << credentials.to_string();
    return os;
}

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _net_http_credentials_hpp
