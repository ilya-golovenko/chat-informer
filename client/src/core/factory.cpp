//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/factory.hpp>
#include <core/exception.hpp>

// MISSIO headers
#include <missio/logging/common.hpp>


namespace missio
{

void factory::create_instances()
{
    LOG_NOTICE("creating instances");

    storage_.reset(new missio::storage());
    informer_.reset(new missio::informer());
    downloader_.reset(new net::http::downloader());
}

void factory::destroy_instances()
{
    LOG_NOTICE("destroying instances");

    downloader_.reset();
    informer_.reset();
    storage_.reset();
}

storage& factory::storage()
{
    if(!storage_)
        throw exception("storage instance is not initialized");

    return *storage_;
}

informer& factory::informer()
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

boost::scoped_ptr<missio::storage> factory::storage_;
boost::scoped_ptr<missio::informer> factory::informer_;
boost::scoped_ptr<net::http::downloader> factory::downloader_;

}   // namespace missio
