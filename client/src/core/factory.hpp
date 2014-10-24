//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_factory_hpp
#define _chat_core_factory_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/storage.hpp>
#include <core/informer.hpp>
#include <network/http/downloader.hpp>

// STL headers
#include <memory>


namespace chat
{

class factory
{
public:
    factory() = delete;
    ~factory() = delete;

    static void create_instances();
    static void destroy_instances();

    static chat::storage& storage();
    static chat::informer& informer();
    static net::http::downloader& downloader();

private:
    static std::unique_ptr<chat::storage> storage_;
    static std::unique_ptr<chat::informer> informer_;
    static std::unique_ptr<net::http::downloader> downloader_;
};

}   // namespace chat

#endif  // _chat_core_factory_hpp
