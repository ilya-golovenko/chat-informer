//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/request.hpp>
#include <network/http/common.hpp>
#include <missio/format/write.hpp>


namespace chat
{
namespace net
{
namespace http
{

request::request() :
    method_(request_methods::get)
{
}

request::request(version const& version) :
    message(version),
    method_(request_methods::get)
{
}

request::request(std::string const& method, uri const& uri) :
    method_(method),
    uri_(uri)
{
}

request::request(std::string const& method, uri const& uri, version const& version) :
    message(version),
    method_(method),
    uri_(uri)
{
}

uri const& request::get_uri() const
{
    return uri_;
}

void request::set_uri(uri const& uri)
{
    uri_ = uri;
}

std::string const& request::get_method() const
{
    return method_;
}

void request::set_method(std::string const& method)
{
    method_ = method;
}

void request::set_host(std::string const& host)
{
    set(header_names::host, host);
}

void request::set_host(std::string const& host, std::uint16_t port)
{
    std::string value = host;

    if(http_port_number != port)
        missio::format::write(value, ":", port);

    set_host(value);
}

boost::optional<std::string> request::get_host() const
{
    return get(header_names::host);
}

void request::set_user_agent(std::string const& user_agent)
{
    set(header_names::user_agent, user_agent);
}

boost::optional<std::string> request::get_user_agent() const
{
    return get(header_names::user_agent);
}

void request::set_cookie(std::string const& cookie)
{
    set(header_names::cookie, cookie);
}

boost::optional<std::string> request::get_cookie() const
{
    return get(header_names::cookie);
}

void request::serialize_to_stream(std::ostream& os) const
{
    os << method_;
    os << strings::space;
    os << uri_;
    os << strings::space;
    os << version_;
    os << strings::crlf;

    message::serialize(os);
}

unique_buffer request::to_unique_buffer() const
{
    std::ostringstream buffer;

    serialize(buffer);

    return make_unique_buffer(buffer.str());
}

shared_buffer request::to_shared_buffer() const
{
    std::ostringstream buffer;

    serialize(buffer);

    return make_shared_buffer(buffer.str());
}

}   // namespace http
}   // namespace net
}   // namespace chat
