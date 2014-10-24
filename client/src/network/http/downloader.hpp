//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _net_http_downloader_hpp
#define _net_http_downloader_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/download_queue.hpp>
//#include <network/http/client_session.hpp>
//#include <network/util/thread_pool.hpp>

// MISSIO headers
#include <missio/logging/common.hpp>

// STL headers
#include <string>
#include <mutex>


namespace chat
{
namespace net
{
namespace http
{

/// Downloader class.
/// Downloads data from HTTP servers
class downloader
{
friend class download;

public:
    /// Creates new instance of downloader
    downloader();

    /// Destroys downloader instance.
    /// Cancels any unfinished downloads
    ~downloader();

    /// Copying is prohibited
    downloader(downloader const&) = delete;
    downloader& operator=(downloader const&) = delete;

    /// Starts downloader instance.
    /// Starts background processing of downloads
    void start();

    /// Stops downloader instance.
    /// Cancels any background processing
    void stop();

    /// Sets HTTP proxy server information for downloader instance
    //void set_proxy_settings(proxy_settings const& proxy_settings);

    /// Adds new download to be processed by downloader
    void add_download(download::pointer download);

private:
    /// Starts next download in the list
    void start_next_download();

    /// Starts specified download
    void start_download(download::pointer download);
    
    /// Called by download object on completion
    void download_complete();

private:
    /// Flag indicating whether download is in progress
    bool busy_;

    /// Flag indicating downloader running state
    bool stopped_;

    /// Data synchronization mutex
    boost::mutex mutex_;

    /// Download queue
    download_queue downloads_;

    /// I/O service object
    boost::asio::io_service io_service_;

    /// List of background processing threads
    //util::thread_pool io_threads_;

    /// Client session instance
    //client_session::pointer session_;
};

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _net_http_downloader_hpp
