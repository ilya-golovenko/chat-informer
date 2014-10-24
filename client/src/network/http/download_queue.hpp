//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _net_http_download_queue_hpp
#define _net_http_download_queue_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/http/download.hpp>

// STL headers
#include <list>


namespace chat
{
namespace net
{
namespace http
{

/// Download list class
class download_queue
{
public:
    /// Creates empty download queue
    download_queue() = default;

    /// Destroys download queue.
    /// Does not cancel any downloads
    ~download_queue() = default;

    /// Copying is prohibited
    download_queue(download_queue const&) = delete;
    download_queue& operator=(download_queue const&) = delete;

    /// Removes all downloads from the queue.
    /// Does not cancel any downloads
    void clear();

    /// Cancels all downloads an clears the queue.
    void cancel();

    /// Returns true if download queue is empty
    bool empty() const;

    /// Returns number of downloads in the queue
    std::size_t size() const;

    /// Returns pointer to the front download
    download::pointer front() const;

    /// Returns pointer to the back download
    download::pointer back() const;

    /// Adds new download to the front of the queue
    void push_front(download::pointer download);

    /// Adds new download to the back of the queue
    void push_back(download::pointer download);

    /// Removes download from the front of the queue.
    /// Does not cancel removed download
    void pop_front();

    /// Removes download from the back of the queue.
    /// Does not cancel removed download
    void pop_back();

    /// Removes download from the queue.
    /// Does not cancel removed download
    void remove(download::pointer download);

private:
    typedef std::list<download::pointer> download_list;

private:
    download_list downloads_;
};

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _net_http_download_queue_hpp
