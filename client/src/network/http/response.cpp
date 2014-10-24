//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/response.hpp>
#include <network/http/common.hpp>
#include <network/mime/types.hpp>

// MISSIO headers
#include <missio/format/format.hpp>


namespace chat
{
namespace net
{
namespace http
{

static missio::format::string const format
{
    "<html>\r\n"
    "<head><title>{0}</title></head>\r\n"
    "<body>\r\n"
    "<center><h1>{0}</h1><center>\r\n"
    "<hr>\r\n"
    "<center>{1}</center>\r\n"
    "</body>\r\n"
    "</html>\r\n"
};

static std::string const padding
{
    "<!-- The padding to disable MSIE's friendly error page -->\r\n"
    "<!-- The padding to disable MSIE's friendly error page -->\r\n"
    "<!-- The padding to disable MSIE's friendly error page -->\r\n"
    "<!-- The padding to disable MSIE's friendly error page -->\r\n"
    "<!-- The padding to disable MSIE's friendly error page -->\r\n"
    "<!-- The padding to disable MSIE's friendly error page -->\r\n"
    "<!-- The padding to disable MSIE's friendly error page -->\r\n"
};

response::response() :
    status_(status::ok)
{
}

response::response(status status) :
    status_(status)
{
}

response::response(version const& version, status const& status) :
    message(version),
    status_(status)
{
}

status const& response::get_status() const
{
    return status_;
}

void response::set_status(status const& status)
{
    status_ = status;
}

void response::set_server(std::string const& server)
{
    set(header_names::server, server);
}

boost::optional<std::string> response::get_server() const
{
    return get(header_names::server);
}

void response::set_cookie(std::string const& cookie)
{
    set(header_names::set_cookie, cookie);
}

boost::optional<std::string> response::get_cookie() const
{
    return get(header_names::set_cookie);
}

void response::serialize(std::ostream& os) const
{
    os << version_;
    os << strings::space;
    os << status_;
    os << strings::crlf;

    message::serialize(os);
}

unique_buffer response::to_unique_buffer() const
{
    std::ostringstream buffer;

    serialize(buffer);

    return make_unique_buffer(buffer.str());
}

shared_buffer response::to_shared_buffer() const
{
    std::ostringstream buffer;

    serialize(buffer);

    return make_shared_buffer(buffer.str());
}

response response::make_stock_response(status const& status, std::string const& server)
{
    std::string content;

    missio::format::format(content, format, status, server);

    response response(status);

    response.add_content(content);
    response.add_content(padding);

    response.set_connection(connection_tokens::close);
    response.set_content_type(mime::text_html);
    response.set_content_length_header();
    response.set_server(server);

    return response;
}

}   // namespace http
}   // namespace net
}   // namespace chat
