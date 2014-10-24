//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/impl/session_base.hpp>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

tcp::connection& session_base::get_tcp_connection()
{
    return tcp_connection_;
}

tcp::connection const& session_base::get_tcp_connection() const
{
    return tcp_connection_;
}

std::chrono::seconds session_base::get_read_timeout() const
{
    return read_timeout_;
}

void session_base::set_read_timeout(std::chrono::seconds timeout)
{
    read_timeout_ = timeout;
}

std::chrono::seconds session_base::get_keep_alive_timeout() const
{
    return keep_alive_timeout_;
}

void session_base::set_keep_alive_timeout(std::chrono::seconds timeout)
{
    keep_alive_timeout_ = timeout;
}

void session_base::close(bool force)
{
    if(connection_.is_open())
    {
        if(force || !writing_)
        {
            LOG_COMP_DEBUG(session_base, "closing session");

            tcp_connection_.close();
            timeout_timer_.cancel();

            writing_ = false;
            closing_ = false;

            write_buffers_.clear();
            read_buffer_.consume(read_buffer_.size());
        }
        else if(!closing_)
        {
            LOG_COMP_DEBUG(session_base, "request to close session");
            closing_ = true;
        }
    }
}

bool session_base::is_open() const
{
    return !closing_ && tcp_connection_.is_open();
}

session_base::session_base(tcp::connection&& tcp_connection) :
    writing_(false),
    closing_(false),
    keep_alive_(false),
    read_timeout_(60),
    keep_alive_timeout_(120),
    chunked_encoding_(false),
    chunk_size_(0u),
    content_expected_(true),
    content_read_(0ull),
    content_length_(0ull),
    tcp_connection_(std::move(tcp_connection)),
    timeout_timer_(tcp_connection_)
{
}

bool session_base::is_keep_alive() const
{
    return keep_alive_;
}

bool session_base::is_chunked_encoding() const
{
    return chunked_encoding_;
}

bool session_base::is_content_expected(request const& request) const
{
    std::string const& method = request.get_method();

    if(request_methods::head == method ||
        (request_methods::put != method &&
        request_methods::post != method))
    {
        return false;
    }

    return true;
}

bool session_base::is_content_expected(response const& response) const
{
    status const status = response.get_status();

    if(status.is_provisional() ||
        status::no_content == status ||
        status::reset_content == status ||
        status::not_modified == status)
    {
        return false;
    }

    return content_expected_;
}

bool session_base::is_reading_completed() const
{
    if(!content_expected_ ||
        content_length_ == content_read_ ||
        (chunked_encoding_ && 0u == chunk_size_))
    {
        return true;
    }

    return false;
}

bool session_base::is_keep_alive(message const& message)
{
    if(!message.get_content_length())
    {
        if(!is_chunked_encoding(message))
            return false;
    }

    boost::optional<std::string> connection = message.get_connection();

    if(!connection)
        connection = message.get_proxy_connection();

    if(connection)
        return boost::algorithm::iequals(*connection, connection_tokens::keep_alive);

    return message.get_version() >= version(1, 1);
}

bool session_base::is_chunked_encoding(message const& message)
{
    boost::optional<std::string> encoding = message.get_transfer_encoding();

    if(encoding)
        return boost::algorithm::iequals(*encoding, encoding_tokens::chunked);

    return false;
}

std::uint64_t session_base::get_content_length(message const& message)
{
   boost::optional<std::uint64_t> length = message.get_content_length();

    if(!length)
        return std::numeric_limits<std::uint64_t>::max();

    return *length;
}

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat
