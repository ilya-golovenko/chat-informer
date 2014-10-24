//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------


template <typename Session>
void basic_session<Session>::write_content(unique_buffer&& buffer)
{
    begin_write_content(write_buffer(std::forward<unique_buffer>(buffer)));
}

template <typename Session>
void basic_session<Session>::write_content(shared_buffer const& buffer)
{
    begin_write_content(write_buffer(buffer));
}

template <typename Session>
basic_session<Session>::basic_session(tcp::connection&& connection) :
    detail::session_base(std::forward<tcp::connection>(connection))
{
}

template <typename Session>
void basic_session<Session>::begin_write_message(write_message const& message)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    if(!writing_)
    {
        LOG_COMP_DEBUG(basic_session, "writing message");

        writing_ = true;

        keep_alive_ = is_keep_alive(message);
        chunked_encoding_ = is_chunked_encoding(message);
        content_expected_ = is_content_expected(message);

        write_buffers_.push(write_buffer(message.to_unique_buffer()));

        connection_.async_write(write_buffers_.front().const_buffer(), bind_handler(shared_from_this(), &basic_session::handle_write));
    }
    else
    {
        LOG_COMP_ERROR(basic_session, "write operation is already in progress");
        process_completed(boost::asio::error::in_progress);
    }
}

template <typename Session>
void basic_session<Session>::begin_write_content(write_buffer&& buffer)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    write_buffers_.push(std::forward<write_buffer>(buffer));

    if(!writing_)
    {
        LOG_COMP_DEBUG(basic_session, "writing content");

        writing_ = true;

        if(chunked_encoding_)
        {
            std::size_t const size = write_buffers_.front().size();

            missio::format::write(chunk_header_, missio::format::hex(size));

            std::array<boost::asio::const_buffer, 4> write_buffers =
            {
                boost::asio::buffer(chunk_header_),
                boost::asio::buffer(strings::crlf),
                write_buffers_.front().const_buffer(),
                boost::asio::buffer(strings::crlf)
            };

            connection_.async_write(write_buffers, bind_handler(shared_from_this(), &basic_session::handle_write));
        }
        else
        {
            connection_.async_write(write_buffers_.front().const_buffer(), bind_handler(shared_from_this(), &basic_session::handle_write));
        }
    }
}

template <typename Session>
void basic_session<Session>::begin_read_message()
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_DEBUG(basic_session, "reading message");

    chunk_size_ = 0;
    content_read_ = 0;

    if(read_timeout_ > 0)
        timer_.start(read_timeout_);

    connection_.async_read(read_buffer_, strings::crlf_crlf, bind_handler(shared_from_this(), &basic_session::handle_read_message));
}

template <typename Session>
void basic_session<Session>::begin_read_content()
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    if(read_timeout_ > 0)
        timer_.start(read_timeout_);

    if(chunked_encoding_)
    {
        LOG_COMP_DEBUG(basic_session, "reading chunk header");
        connection_.async_read(read_buffer_, strings::crlf, bind_handler(shared_from_this(), &basic_session::handle_read_chunk_header));
    }
    else
    {
        LOG_COMP_DEBUG(basic_session, "reading content");
        connection_.async_read(read_buffer_.prepare(8192), bind_handler(shared_from_this(), &basic_session::handle_read_content));
    }
}

template <typename Session>
void basic_session<Session>::begin_read_chunk_data()
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_DEBUG(basic_session, "reading chunk data");

    connection_.async_read(read_buffer_.prepare(chunk_size_ + 2), bind_handler(shared_from_this(), &basic_session::handle_read_chunk_data));
}

template <typename Session>
void basic_session<Session>::begin_read_trailer_headers()
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_DEBUG(basic_session, "reading trailer headers");

    if(read_timeout_ > 0)
        timer_.start(read_timeout_);

    connection_.async_read(read_buffer_, strings::crlf, bind_handler(shared_from_this(), &basic_session::handle_read_trailer_headers));
}

template <typename Session>
bool basic_session<Session>::process_message_read(read_message const& message)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    keep_alive_ = is_keep_alive(message);
    chunked_encoding_ = is_chunked_encoding(message);
    content_expected_ = is_content_expected(message);
    content_length_ = get_content_length(message);

    try
    {
        message_signal_(message);
    }
    catch(std::exception const& e)
    {
        LOG_COMP_WARNING(basic_session, e);
    }
}

template <typename Session>
void basic_session<Session>::process_content_read(content_range const& content)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    content_read_ += content.size();

    try
    {
        content_signal_(content);
    }
    catch(std::exception const& e)
    {
        LOG_COMP_WARNING(basic_session, e);
    }
}

template <typename Session>
void basic_session<Session>::process_completed(boost::system::error_code const& error)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    if(error)
    {
        close(true);
    }
    else
    {
        if(keep_alive_)
            timer_.start(keep_alive_timeout_);
        else
            close(false);
    }

    try
    {
        completion_signal_(error);
    }
    catch(std::exception const& e)
    {
        LOG_COMP_WARNING(basic_session, e);
    }
}

template <typename Session>
void basic_session<Session>::handle_write(boost::system::error_code const& error, std::size_t bytes_transferred)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_TRACE(basic_session, bytes_transferred, " bytes has been written");

    statistics::add_bytes_written(bytes_transferred);

    if(!error)
    {
        write_buffers_.pop();

        if(write_buffers_.empty())
        {
            writing_ = false;

            if(closing_)
                close(true);
        }
        else
        {
            begin_write_content();
        }
    }
    else
    {
        LOG_COMP_DEBUG(basic_session, "cannot write data: ", error);
        process_completed(error);
    }
}

template <typename Session>
void basic_session<Session>::handle_read_message(boost::system::error_code const& error, std::size_t bytes_transferred)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_TRACE(basic_session, bytes_transferred, " bytes has been read");

    statistics::add_bytes_read(bytes_transferred);

    if(!error || error == boost::asio::error::eof)
    {
        read_message message;

        const_buffers_type buffers = read_buffer_.data();
        std::size_t size = boost::asio::buffer_size(buffers);

        buffers_iterator begin = boost::asio::buffers_begin(buffers);
        buffers_iterator end = boost::asio::buffers_end(buffers);

        if(session_traits<Session>::parse_message(message, begin, end))
        {
            timer_.cancel();

            process_message_read(message);

            if(chunked_encoding_)
            {
                size -= std::distance(begin, end);
                read_buffer_.consume(size);
            }
            else
            {
                content_range content(begin, end);
                process_content_read(content);
                read_buffer_.consume(size);
            }

            if(is_reading_completed())
            {
                LOG_COMP_DEBUG(basic_session, "message has been read successfully");
                process_completed(boost::system::error_code());
            }
            else if(error == boost::asio::error::eof)
            {
                LOG_COMP_DEBUG(basic_session, "session has been aborted: ", error);
                process_completed(error);
            }
            else
            {
                begin_read_content();
            }
        }
        else
        {
            LOG_COMP_DEBUG(basic_session, "cannot parse message");
            process_completed(boost::asio::error::invalid_argument);
        }
    }
    else
    {
        LOG_COMP_DEBUG(basic_session, "cannot read message: ", error);
        process_completed(error);
    }
}

template <typename Session>
void basic_session<Session>::handle_read_content(boost::system::error_code const& error, std::size_t bytes_transferred)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_TRACE(basic_session, bytes_transferred, " bytes has been read");

    statistics::add_bytes_read(bytes_transferred);

    if(!error || error == boost::asio::error::eof)
    {
        timer_.cancel();

        read_buffer_.commit(bytes_transferred);

        const_buffers_type buffers = read_buffer_.data();
        std::size_t size = boost::asio::buffer_size(buffers);

        buffers_iterator begin = boost::asio::buffers_begin(buffers);
        buffers_iterator end = boost::asio::buffers_end(buffers);

        content_range content(begin, end);
        process_content_read(content);
        read_buffer_.consume(size);

        if(is_reading_completed())
        {
            LOG_COMP_DEBUG(basic_session, "message has been read successfully");
            process_completed(boost::system::error_code());
        }
        else if(error == boost::asio::error::eof)
        {
            LOG_COMP_DEBUG(basic_session, "session has been aborted: ", error);
            process_completed(error);
        }
        else
        {
            begin_read_content();
        }
    }
    else
    {
        LOG_COMP_DEBUG(basic_session, "cannot read message: ", error);
        process_completed(error);
    }
}

template <typename Session>
void basic_session<Session>::handle_read_chunk_header(boost::system::error_code const& error, std::size_t bytes_transferred)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_TRACE(basic_session, bytes_transferred, " bytes has been read");

    statistics::add_bytes_read(bytes_transferred);

    if(!error || error == boost::asio::error::eof)
    {
        const_buffers_type buffers = read_buffer_.data();
        std::size_t size = boost::asio::buffer_size(buffers);

        buffers_iterator begin = boost::asio::buffers_begin(buffers);
        buffers_iterator end = boost::asio::buffers_end(buffers);

        if(detail::parse_chunk_header(chunk_size_, begin, end))
        {
            size -= std::distance(begin, end);
            read_buffer_.consume(size);

            if(0u == chunk_size_)
            {
                begin_read_trailer_headers();
            }
            else if(error == boost::asio::error::eof)
            {
                LOG_COMP_DEBUG("session has been aborted: ", error);
                process_completed(error);
            }
            else if(begin + chunk_size_ + 2 <= end)
            {
                content_range content(begin, begin + chunk_size_);
                process_content_read(content);
                read_buffer_.consume(chunk_size_ + 2);
                begin_read_content();
            }
            else
            {
                begin_read_chunk_data();
            }
        }
        else
        {
            LOG_COMP_DEBUG("cannot parse chunk header: ", error);
            process_completed(error);
        }
    }
    else
    {
        LOG_COMP_DEBUG(basic_session, "cannot read content: ", error);
        process_completed(error);
    }
}

template <typename Session>
void basic_session<Session>::handle_read_chunk_data(boost::system::error_code const& error, std::size_t bytes_transferred)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_TRACE(basic_session, bytes_transferred, " bytes has been read");

    statistics::add_bytes_read(bytes_transferred);

    if(!error || error == boost::asio::error::eof)
    {
        timer_.cancel();

        read_buffer_.commit(bytes_transferred);
        const_buffers_type buffers = read_buffer_.data();

        buffers_iterator begin = boost::asio::buffers_begin(buffers);
        buffers_iterator end = boost::asio::buffers_end(buffers);

        if(begin + chunk_size_ + 2 <= end)
        {
            content_range content(begin, begin + chunk_size_);
            process_content_read(content);
            read_buffer_.consume(chunk_size_ + 2);
            begin_read_content();
        }
        else
        {
            begin_read_chunk_data();
        }
    }
    else
    {
        LOG_COMP_DEBUG(basic_session, "cannot read content: ", error);
        process_completed(error);
    }
}

template <typename Session>
void basic_session<Session>::handle_read_trailer_headers(boost::system::error_code const& error, std::size_t bytes_transferred)
{
    LOG_COMP_TRACE_FUNCTION(basic_session);

    LOG_COMP_TRACE(basic_session, bytes_transferred, " bytes has been read");

    statistics::add_bytes_read(bytes_transferred);

    if(!error || error == boost::asio::error::eof)
    {
        timer_.cancel();

        read_buffer_.consume(bytes_transferred);

        LOG_COMP_DEBUG(basic_session, "message has been read successfully");
        process_completed(boost::system::error_code());
    }
    else
    {
        LOG_COMP_DEBUG(basic_session, "cannot read content: ", error);
        process_completed(error);
    }
}
