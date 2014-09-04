//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_credentials_hpp
#define _chat_core_credentials_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <iostream>
#include <string>


namespace chat
{

class credentials
{
public:
    credentials();
    ~credentials();

    credentials(std::wstring const& nickname,
                std::wstring const& password);

    credentials(credentials&& other);
    credentials& operator=(credentials&& other);    

    credentials(credentials const& other);
    credentials& operator=(credentials const& other);    

    std::string to_string() const;
    std::wstring to_wstring() const;

private:
    static std::string make_credentials(std::string const& nickname,
                                        std::string const& password);

    static std::string make_credentials(std::wstring const& nickname,
                                        std::wstring const& password);

private:
    std::string credentials_;
};

inline std::ostream& operator<<(std::ostream& os, credentials const& credentials)
{
    os << credentials.to_string();
    return os;
}

}   // namespace chat

#endif  // _chat_core_credentials_hpp
