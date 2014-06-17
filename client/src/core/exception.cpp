//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/exception.hpp>

// MISSIO headers
#include <missio/utf8/convert.hpp>


namespace missio
{

exception::exception(std::string const& message) :
    message_(message)
{
}

exception::exception(std::wstring const& message) :
    message_(utf8::convert(message))
{
}

exception::~exception() throw()
{
}

char const* exception::what() const throw()
{
    return message_.c_str();
}

std::string const& exception::message() const throw()
{
    return message_;
}

}   // namespace missio
