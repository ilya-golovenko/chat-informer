//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/message.hpp>
#include <network/http/common.hpp>

// BOOST headers
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

// STL headers
#include <iterator>


namespace chat
{
namespace net
{
namespace http
{

message::message(version const& version) :
    version_(version)
{
}

void message::add_header(std::string const& name, std::string const& value)
{
    headers_.emplace(name, value);
}

void message::set_header(std::string const& name, std::string const& value)
{
    erase_header(name);
    add_header(name, value);
}

boost::optional<std::string> message::get_header(std::string const& name) const
{
    for(header const& header : headers_)
    {
        if(boost::algorithm::iequals(header.get_name(), name))
        {
            return boost::make_optional(header.get_value());
        }
    }

    return boost::optional<std::string>();
}

std::vector<std::string> message::get_headers(std::string const& name) const
{
    std::vector<std::string> values;

    for(header const& header : headers_)
    {
        if(boost::algorithm::iequals(header.get_name(), name))
        {
            headers.push_back(header.get_value());
        }
    }

    return values;
}

void message::erase_header(std::string const& name)
{
    headers_.erase(std::remove_if(headers_.begin(), headers_.end(), [](header const& header)
        { return boost::algorithm::iequals(header.get_name(), name); }), headers_.end()); 
}

void message::clear()
{
    headers_.clear();
    content_.clear();
}

version const& message::get_version() const
{
    return version_;
}

void message::set_version(version const& version)
{
    version_ = version;
}

void message::set_version(unsigned int major, unsigned int minor)
{
    set_version(version(major, minor));
}

void message::update_content_length()
{
    set_content_length(content_.size());
}

void message::set_content_length(boost::uint64_t length)
{
    set(header_names::content_length, length);
}

boost::optional<boost::uint64_t> message::get_content_length() const
{
    boost::optional<std::string> value = get(header_names::content_length);

    if(value)
        return boost::lexical_cast<boost::uint64_t>(*value);

    return boost::optional<boost::uint64_t>();
}

void message::set_content_type(std::string const& mime_type)
{
    set(header_names::content_type, mime_type);
}

boost::optional<std::string> message::get_content_type() const
{
    return get(header_names::content_type);
}

void message::set_transfer_encoding(std::string const& encoding)
{
    set(header_names::transfer_encoding, encoding);
}

boost::optional<std::string> message::get_transfer_encoding() const
{
    return get(header_names::transfer_encoding);
}

void message::set_connection(std::string const& connection)
{
    set(header_names::connection, connection);
}

boost::optional<std::string> message::get_connection() const
{
    return get(header_names::connection);
}

void message::set_proxy_connection(std::string const& connection)
{
    set(header_names::proxy_connection, connection);
}

boost::optional<std::string> message::get_proxy_connection() const
{
    return get(header_names::proxy_connection);
}

bool message::content_empty() const
{
    return content_.empty();
}

std::size_t message::content_size() const
{
    return content_.size();
}

std::string const& message::content() const
{
    return content_;
}

void message::set_content(std::string const& data)
{
    content_.assign(data);
}

void message::add_content(std::string const& data)
{
    content_.append(data);
}

void message::clear_content()
{
    content_.clear();
}

void message::serialize(std::ostream& os) const
{
    for(header const& header : headers_)
    {
        header.serialize(os);
        os << strings::crlf;
    }

    os << strings::crlf;
    os << content_;
}

}   // namespace http
}   // namespace net
}   // namespace chat
