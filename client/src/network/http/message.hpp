//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_message_hpp
#define _chat_network_http_message_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/constants.hpp>
#include <network/http/version.hpp>
#include <network/http/header.hpp>

// BOOST headers
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/asio.hpp>

// STL headers
#include <vector>
#include <string>


namespace chat
{
namespace net
{
namespace http
{

class message
{
public:
    message() = default;
    ~message() = default;

    explicit message(version const& version);

    message(message const&) = default;
    message& operator=(message const&) = default;

    message(message&&) = default;
    message& operator=(message&&) = default;

    void add_header(std::string const& name, std::string const& value);
    void set_header(std::string const& name, std::string const& value);

    template <typename T> void add_header(std::string const& name, T const& value);
    template <typename T> void set_header(std::string const& name, T const& value);

    boost::optional<std::string> get_header(std::string const& name) const;
    std::vector<std::string> get_headers(std::string const& name) const;

    template <typename T> boost::optional<T> get_header(std::string const& name) const;

    void erase_header(std::string const& name);
    void clear_headers();

    version const& get_version() const;
    void set_version(version const& version);
    void set_version(std::uint16_t major, std::uint16_t minor);

    void update_content_length();

    void set_content_length(std::uint64_t length);
    boost::optional<std::uint64_t> get_content_length() const;

    void set_content_type(std::string const& mime_type);
    boost::optional<std::string> get_content_type() const;

    void set_transfer_encoding(std::string const& encoding);
    boost::optional<std::string> get_transfer_encoding() const;

    void set_connection(std::string const& connection);
    boost::optional<std::string> get_connection() const;

    void set_proxy_connection(std::string const& connection);
    boost::optional<std::string> get_proxy_connection() const;

    bool content_empty() const;
    std::size_t content_size() const;
    std::string const& content() const;

    void set_content(std::string const& data);
    void add_content(std::string const& data);

    template <typename Iterator> void set_content(Iterator first, Iterator last);
    template <typename Iterator> void add_content(Iterator first, Iterator last);

    void clear_content();

    void serialize(std::ostream& os) const;

protected:
    typedef std::vector<header> header_vector;

protected:
    version version_;
    header_vector headers_;
    std::string content_;
};

// Implementation headers
#include <network/http/message.inl>

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_message_hpp
