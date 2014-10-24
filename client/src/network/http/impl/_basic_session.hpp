//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_impl_basic_session_hpp
#define _chat_network_http_impl_basic_session_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/impl/session_traits.hpp>
#include <network/http/impl/session_base.hpp>
#include <network/util/write_buffer.hpp>
#include <network/util/bind_handler.hpp>
#include <network/util/statistics.hpp>

// MISSIO headers
#include <missio/format/write.hpp>

// BOOST headers
#include <boost/signals2.hpp>

// STL headers
#include <memory>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

template <typename Session>
class basic_session :
    public session_base,
    public std::enable_shared_from_this<Session>
{
public:
    typedef std::shared_ptr<Session> pointer;

public:
    typedef typename session_traits<Session>::read_message_type read_message;
    typedef typename session_traits<Session>::write_message_type write_message;

public:
    void write_content(unique_buffer&& buffer);
    void write_content(shared_buffer const& buffer);

    template <typename T, typename F>
    void on_message_read(std::shared_ptr<T> object, F function)
    {
        message_signal_.connect(bind_weak_handler(object, function));
    }

    template <typename T, typename F>
    void on_content_read(std::shared_ptr<T> object, F function)
    {
        content_signal_.connect(bind_weak_handler(object, function));
    }

    template <typename T, typename F>
    void on_completion(std::shared_ptr<T> object, F function)
    {
        completion_signal_.connect(bind_weak_handler(object, function));
    }

protected:
    explicit basic_session(tcp::connection&& connection);

    void begin_write_message(write_message const& message);
    void begin_write_content(write_buffer&& buffer);

    void begin_read_message();
    void begin_read_content();
    void begin_read_chunk_data();
    void begin_read_trailer_headers();

private:
    bool process_message_read(read_message const& message);
    void process_content_read(content_range const& content);
    void process_completed(boost::system::error_code const& error);

    void handle_write(boost::system::error_code const& error, std::size_t bytes_transferred);

    void handle_read_message(boost::system::error_code const& error, std::size_t bytes_transferred);
    void handle_read_content(boost::system::error_code const& error, std::size_t bytes_transferred);

    void handle_read_chunk_header(boost::system::error_code const& error, std::size_t bytes_transferred);
    void handle_read_chunk_data(boost::system::error_code const& error, std::size_t bytes_transferred);    

    void handle_read_trailer_headers(boost::system::error_code const& error, std::size_t bytes_transferred);

private:
    typedef boost::signals2::signal<bool (read_message const&)> message_signal_type;
    typedef boost::signals2::signal<bool (content_range const&)> content_signal_type;
    typedef boost::signals2::signal<void (boost::system::error_code const&)> complete_signal_type;

private:
    message_signal_type message_signal_;
    content_signal_type content_signal_;
    complete_signal_type completion_signal_;
};

// Implementation headers
#include <network/http/impl/basic_session.inl>

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_impl_basic_session_hpp
