//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/download_queue.hpp>


namespace chat
{
namespace net
{
namespace http
{

void download_queue::clear()
{
    downloads_.clear();
}

void download_queue::cancel()
{
    for(auto download : downloads_)
    {
        download->abort();
    }

    downloads_.clear();
}

bool download_queue::empty() const
{
    return downloads_.empty();
}

std::size_t download_queue::size() const
{
    return downloads_.size();
}

download::pointer download_queue::front() const
{
    return downloads_.front();
}

download::pointer download_queue::back() const
{
    return downloads_.back();
}

void download_queue::push_front(download::pointer download)
{
    downloads_.push_front(download);
}

void download_queue::push_back(download::pointer download)
{
    downloads_.push_back(download);
}

void download_queue::pop_front()
{
    downloads_.pop_front();
}

void download_queue::pop_back()
{
    downloads_.pop_back();
}

void download_queue::remove(download::pointer download)
{
    downloads_.remove(download);
}

}   // namespace http
}   // namespace net
}   // namespace chat
