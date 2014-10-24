//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//--------------------------------------------------------------------------- 

// Application headers
#include <network/http/download.hpp>


namespace chat
{
namespace net
{
namespace http
{

download::download(http::uri const& uri) :
    started_(false),
    completed_(false),
    uri_(uri)
{
}

download::download(http::uri const& uri, request&& request) :
    started_(false),
    completed_(false),
    read_timeout_(60),
    request_(std::move(request)),
    uri_(uri)
{
}

download::~download()
{
}

void download::set_read_timeout(std::chrono::seconds const& timeout)
{
    read_timeout_ = timeout;
}

http::uri const& download::uri() const
{
    return uri_;
}
/*
void download::start(client_session::pointer session)
{
    if(!started_.exchange(true))
    {
        LOG_COMP_DEBUG(download, "starting download: ", uri_);

        request_.set_uri(uri_.relative());
        request_.set_host(uri_.host_info());
        request_.set_content_length_header();

        session->set_handler(shared_from_this());
        session->set_read_timeout(read_timeout_);

        session->start(uri_, std::move(request_));
    }
}
*/
void download::abort()
{
    abort(boost::asio::error::operation_aborted);
}

bool download::is_started() const
{
    return started_;
}

bool download::is_completed() const
{
    return completed_;
}

void download::abort(boost::system::error_code const& error)
{
    if(!completed_.exchange(true))
    {
        LOG_COMP_DEBUG(download, "aborting download with error: ", error);

        //handle_complete(error);
    }
}
/*
void download::on_response(response const& response)
{
    if(!completed_)
    {
        //handle_response(response);
    }
}

void download::on_content(content_range const& content)
{
    if(!completed_)
    {
        //handle_content(content);
    }
}

void download::on_complete(boost::system::error_code const& error)
{
    if(!completed_.exchange(true))
    {
        //handle_complete(error);
    }
}
*/
}   // namespace http
}   // namespace net
}   // namespace chat
