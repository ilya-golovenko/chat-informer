//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/factory.hpp>
#include <core/exception.hpp>

// MISSIO headers
#include <missio/logging/common.hpp>


namespace chat
{

void factory::create_instances()
{
    LOG_COMP_TRACE_FUNCTION(factory);

    LOG_COMP_NOTICE(factory, "creating instances");

    storage_.reset(new chat::storage); // = std::make_unique<chat::storage>();
    informer_.reset(new chat::informer); // = std::make_unique<chat::informer>();
    downloader_.reset(new net::http::downloader); // = std::make_unique<net::http::downloader>();
}

void factory::destroy_instances()
{
    LOG_COMP_TRACE_FUNCTION(factory);

    LOG_COMP_NOTICE(factory, "destroying instances");

    downloader_.reset();
    informer_.reset();
    storage_.reset();
}

chat::storage& factory::storage()
{
    if(!storage_)
        throw exception("storage instance is not initialized");

    return *storage_;
}

chat::informer& factory::informer()
{
    if(!informer_)
        throw exception("informer instance is not initialized");

    return *informer_;
}

net::http::downloader& factory::downloader()
{
    if(!downloader_)
        throw exception("downloader instance is not initialized");

    return *downloader_;
}

std::unique_ptr<chat::storage> factory::storage_;
std::unique_ptr<chat::informer> factory::informer_;
std::unique_ptr<net::http::downloader> factory::downloader_;

}   // namespace chat
