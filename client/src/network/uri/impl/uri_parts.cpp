//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/uri/impl/uri_parts.hpp>


namespace chat
{
namespace net
{
namespace impl
{

bool operator==(uri_parts const& lhs, uri_parts const& rhs)
{
    return lhs.scheme == rhs.scheme &&
           lhs.user_info == rhs.user_info &&
           lhs.host == rhs.host &&
           lhs.port == rhs.port &&
           lhs.path == rhs.path &&
           lhs.query == rhs.query &&
           lhs.fragment == rhs.fragment;
}

bool operator<(uri_parts const& lhs, uri_parts const& rhs)
{
    if(lhs.scheme != rhs.scheme)
        return lhs.scheme < rhs.scheme;

    if(lhs.user_info != rhs.user_info)
        return lhs.user_info < rhs.user_info;

    if(lhs.host != rhs.host)
        return lhs.host < rhs.host;

    if(lhs.port != rhs.port)
        return lhs.port < rhs.port;

    if(lhs.path != rhs.path)
        return lhs.path < rhs.path;

    if(lhs.query != rhs.query)
        return lhs.query < rhs.query;

    return lhs.fragment < rhs.fragment;
}

}   // namespace impl
}   // namespace net
}   // namespace chat
