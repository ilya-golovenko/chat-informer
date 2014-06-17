//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_exception_hpp
#define _missio_exception_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <exception>
#include <string>


namespace missio
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

}   // namespace missio

#endif  // _missio_exception_hpp
