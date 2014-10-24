//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_impl_uri_base_hpp
#define _chat_network_uri_impl_uri_base_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/uri/impl/uri_parts.hpp>

// STL headers
#include <string>


namespace chat
{
namespace net
{
namespace impl
{

class uri_base
{
public:
    uri_base() = default;
    virtual ~uri_base() = default;

    uri_base(std::string const& str);
    uri_base(uri_parts const& parts);

    uri_base(uri_base const&) = default;
    uri_base& operator=(uri_base const&) = default;

    uri_base(uri_base&&) = default;
    uri_base& operator=(uri_base&&) = default;

    bool operator==(uri_base const& other) const;
    bool operator!=(uri_base const& other) const;
    bool operator<(uri_base const& other) const;

protected:
    uri_parts parts_;
};

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_impl_uri_base_hpp
