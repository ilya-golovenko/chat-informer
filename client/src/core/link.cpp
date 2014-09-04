//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/link.hpp>

// STL headers
#include <utility>


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

link::~link()
{
}

link::link(link&& other) :
    uri_(std::move(other.uri_)),
    name_(std::move(other.name_)),
    descr_(std::move(other.descr_))
{
}

link& link::operator=(link&& other)
{
    if(&other != this)
    {
        uri_ = std::move(other.uri_);
        name_ = std::move(other.name_);
        descr_ = std::move(other.descr_);
    }

    return *this;
}

link::link(link const& other) :
    uri_(other.uri_),
    name_(other.name_),
    descr_(other.descr_)
{
}

link& link::operator=(link const& other)
{
    if(&other != this)
    {
        uri_ = other.uri_;
        name_ = other.name_;
        descr_ = other.descr_;
    }

    return *this;
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
