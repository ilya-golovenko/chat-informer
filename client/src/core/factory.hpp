//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_factory_hpp
#define _missio_core_factory_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/storage.hpp>
#include <core/informer.hpp>
#include <net/http/downloader.hpp>

// BOOST headers
#include <boost/scoped_ptr.hpp>


namespace missio
{

class factory
{
public:
    static void create_instances();
    static void destroy_instances();

    static missio::storage& storage();
    static missio::informer& informer();
    static net::http::downloader& downloader();

private:
    factory();
    ~factory();

private:
    static boost::scoped_ptr<missio::storage> storage_;
    static boost::scoped_ptr<missio::informer> informer_;
    static boost::scoped_ptr<net::http::downloader> downloader_;
};

}   // namespace missio

#endif  // _missio_core_factory_hpp
