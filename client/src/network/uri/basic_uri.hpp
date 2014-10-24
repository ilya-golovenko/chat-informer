//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_basic_uri_hpp
#define _chat_network_uri_basic_uri_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/uri/impl/uri_base.hpp>

// STL headers
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <string>


namespace chat
{
namespace net
{

template <typename Traits>
class basic_uri : public impl::uri_base
{
public:
    basic_uri() = default;
    ~basic_uri() = default;

    basic_uri(std::string const& str);
    basic_uri(impl::uri_parts const& parts);

    basic_uri(basic_uri const&) = default;
    basic_uri& operator=(basic_uri const&) = default;

    basic_uri(basic_uri&&) = default;
    basic_uri& operator=(basic_uri&&) = default;

    std::string scheme() const;
    std::string user_info() const;
    std::string host() const;
    std::uint16_t port() const;
    std::string path() const;
    std::string query() const;
    std::string fragment() const;

    std::string host_info() const;
    std::string authority() const;
    std::string relative() const;

    std::string to_string() const;

public:
    using uri_base::operator==;
    using uri_base::operator!=;
    using uri_base::operator<;
};

template <typename Traits>
inline std::ostream& operator<<(std::ostream& os, basic_uri<Traits> const& uri)
{
    os << uri.to_string();
    return os;
}

// Implementation headers
#include <network/uri/basic_uri.inl>

}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_basic_uri_hpp
