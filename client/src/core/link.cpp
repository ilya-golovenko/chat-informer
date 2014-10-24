//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/link.hpp>


namespace chat
{

link::link(std::wstring const& uri,
           std::wstring const& name,
           std::wstring const& descr) :
    uri_(uri),
    name_(name),
    descr_(descr)
{
}

std::wstring const& link::uri() const
{
    return uri_;
}

std::wstring const& link::name() const
{
    return name_;
}

std::wstring const& link::descr() const
{
    return descr_;
}

}   // namespace chat
