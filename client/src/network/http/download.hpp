//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//--------------------------------------------------------------------------- 
#ifndef _chat_net_http_download_hpp
#define _chat_net_http_download_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/uri.hpp>
#include <network/http/request.hpp>
#include <network/http/response.hpp>
//#include <network/http/client_session.hpp>

// BOOST headers
#include <boost/asio.hpp>

// STL headers
#include <memory>
#include <atomic>
#include <chrono>


namespace chat
{
namespace net
{
namespace http
{

class download/* :
    public client_handler,
    public boost::enable_shared_from_this<download>*/
{
public:
    typedef boost::shared_ptr<download> pointer;

public:
    /// Creates new download
    explicit download(http::uri const& uri);

    /// Creates new download
    download(http::uri const& uri, request&& request);

    /// Destroys download instance
    virtual ~download();

    /// Sets read operation timeout (in seconds)
    void set_read_timeout(std::chrono::seconds const& timeout);

    /// HTTP uri used for downloading
    http::uri const& uri() const;

    /// Starts downloading.
    /// Uses specified session object to download from HTTP server
    //void start(client_session::pointer session);

    /// Aborts downloading.
    /// Completes download with error code
    /// and sets to boost::asio::error::operation_aborted
    void abort();

    /// Returns true if download is started
    bool is_started() const;

    /// Returns true if download is finished
    bool is_completed() const;

protected:
    /// Aborts download with the specified error code
    void abort(boost::system::error_code const& error);

protected:
    // client: write request -> write content -> read response -> read content
    //virtual void on_request_written();
    //virtual void on_content_written();
    //virtual void on_response_read(response const& response);
    //virtual void on_content_read(content_range const& content);
    //virtual void on_complete(boost::system::error_code const& error);

    // server: read request -> read content -> write response -> write content
    //virtual void on_request_read(request const& request);
    //virtual void on_content_read(content_range const& content);
    //virtual void on_response_written();
    //virtual void on_content_written();
    //virtual void on_complete(boost::system::error_code const& error);

    /// Called by session object when HTTP response is available
    virtual void on_response(response const& response);

    /// Called by session object when new content data is available
    //virtual void on_content(content_range const& content);

    /// Called by session object when the download has completed or failed
    virtual void on_complete(boost::system::error_code const& error);

private:
    /// Flag indicating whether download is started
    mutable std::atomic_bool started_;

    /// Flag indicating whether download is completed
    mutable std::atomic_bool completed_;

    /// Read operation timeout, in seconds
    std::chrono::seconds read_timeout_;

    /// HTTP request
    request request_;

    /// HTTP uri
    http::uri uri_;
};

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_net_http_download_hpp
