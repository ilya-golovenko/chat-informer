//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/downloader.hpp>


namespace chat
{
namespace net
{
namespace http
{

downloader::downloader() :
    busy_(false),
    stopped_(true)/*,
    io_threads_(io_service_)*/
{
    //session_ = client_session::create(io_service_);
    //TODO: session_->on_completion(&downloadert::on_download_completion);
}

downloader::~downloader()
{
    //session_.reset();
    //io_threads_.stop();
    downloads_.cancel();
}

void downloader::start()
{
    std::lock_guard<std::mutex> lock(mutex_);

    if(stopped_)
    {
        LOG_COMP_INFO(downloader, "starting");
        stopped_ = false;

        //io_threads_.start(1);
        start_next_download();
    }
}

void downloader::stop()
{
    std::lock_guard<std::mutex> lock(mutex_);

    if(!stopped_)
    {
        LOG_COMP_INFO(downloader, "stopping");
        stopped_ = true;

        //io_threads_.stop();
        downloads_.cancel();
/*
        if(session_->is_open())
        {
            LOG_COMP_DEBUG(downloader, "closing session");
            session_->close(true);
        }
    }
*/
}
/*
void downloader::set_proxy_settings(proxy_settings const& proxy_settings)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if(stopped_)
    {
        LOG_NOTICE("setting proxy settings");
        session_->set_proxy_settings(proxy_settings);
    }
}
*/
void downloader::add_download(download::pointer download)
{
    std::lock_guard<std::mutex> lock(mutex_);

    LOG_COMP_DEBUG(downloader, "adding download: ", download->uri());

    if(busy_ || stopped_)
        downloads_.push_back(download);
    else
        start_download(download);
}

void downloader::start_next_download()
{
    if(!busy_ && !downloads_.empty())
    {
        start_download(downloads_.front());
        downloads_.remove(downloads_.front());
    }
}

void downloader::start_download(download::pointer download)
{
    try
    {
        //download->start(session_);
        busy_ = true;
    }
    catch(std::exception const& e)
    {
        LOG_COMP_ERROR(downloader, e);
    }
}

void downloader::download_complete()
{
    std::lock_guard<std::mutex> lock(mutex_);

    busy_ = false;

    if(!stopped_)
        start_next_download();
}

}   // namespace http
}   // namespace net
}   // namespace chat
