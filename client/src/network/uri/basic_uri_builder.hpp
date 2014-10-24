//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_basic_uri_builder_hpp
#define _chat_network_uri_basic_uri_builder_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/uri/impl/uri_builder_base.hpp>
#include <network/uri/basic_uri.hpp>

// STL headers
#include <string>


namespace chat
{
namespace net
{

template <typename Traits>
class basic_uri_builder : public impl::uri_builder_base
{
public:
    basic_uri_builder() = default;
    ~basic_uri_builder() = default;

    explicit basic_uri_builder(std::string const& str);
    explicit basic_uri_builder(basic_uri<Traits> const& uri);

    basic_uri<Traits> uri() const;
};

}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_basic_uri_builder_hpp
