//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_exception_hpp
#define _chat_exception_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <exception>
#include <string>


namespace chat
{

class exception :
    public std::exception
{
public:
    explicit exception(std::string const& message);
    explicit exception(std::wstring const& message);

    virtual ~exception() throw();

    virtual char const* what() const throw();
    std::string const& message() const throw();

private:
    std::string message_;
};

}   // namespace chat

#endif  // _chat_exception_hpp
