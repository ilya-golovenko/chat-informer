//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/uri/impl/uri_builder_base.hpp>
#include <network/uri/impl/uri_encode.hpp>


namespace chat
{
namespace net
{
namespace impl
{

uri_builder_base::uri_builder_base(uri_base const& uri) :
    components_(uri.components_)
{
}

void uri_builder_base::set_scheme(std::string const& scheme)
{
    components_.scheme = scheme;
}

void uri_builder_base::set_user_info(std::string const& user_info)
{
    components_.user_info = decode(user_info);
}

void uri_builder_base::set_host(std::string const& host)
{
    components_.host = decode(host);
}

void uri_builder_base::set_port(std::uint16_t port)
{
    components_.port = port;
}

void uri_builder_base::set_path(std::string const& path)
{
    components_.path = decode(path);
}

void uri_builder_base::set_query(std::string const& query)
{
    components_.query = decode(query);
}

void uri_builder_base::set_fragment(std::string const& fragment)
{
    components_.fragment = decode(fragment);
}

void uri_builder_base::set_host(boost::asio::ip::address const& address)
{
    if(address.is_v6())
    {
        components_.host = '[' + address.to_v6().to_string() + ']';
    }
    else
    {
        components_.host = address.to_v4().to_string();
    }
}

void uri_builder_base::append_path(std::string const& path)
{
}

}   // namespace impl
}   // namespace net
}   // namespace chat
