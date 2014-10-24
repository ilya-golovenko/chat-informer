//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_impl_uri_builder_base_hpp
#define _chat_network_uri_impl_uri_builder_base_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/uri/impl/uri_base.hpp>

// BOOST headers
#include <boost/asio.hpp>

// STL headers
#include <cstdint>
#include <string>


namespace chat
{
namespace net
{
namespace impl
{

class uri_builder_base
{
public:
    uri_builder_base() = default;
    ~uri_builder_base() = default;

    explicit uri_builder_base(uri_base const& uri);

    uri_builder_base(uri_builder_base const&) = delete;
    uri_builder_base& operator=(uri_builder_base const&) = delete;

    void set_scheme(std::string const& scheme);
    void set_user_info(std::string const& user_info);
    void set_host(std::string const& server);
    void set_port(std::uint16_t port);
    void set_path(std::string const& path);
    void set_query(std::string const& query);
    void set_fragment(std::string const& fragment);

    void set_host(boost::asio::ip::address const& address);

    void append_path(std::string const& path);

    template <typename T>
    void add_query(std::string const& key, T const& value);

protected:
    uri_parts parts_;
};

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_impl_uri_builder_base_hpp
