//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_util_statistics_hpp
#define _chat_network_util_statistics_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <cstdint>
#include <atomic>


namespace chat
{
namespace net
{

class statistics
{
public:
    statistics() = delete;
    ~statistics() = delete;

    static std::size_t get_bytes_read();
    static std::size_t get_bytes_written();

    static void add_bytes_read(std::size_t count);
    static void add_bytes_written(std::size_t count);

private:
    static std::atomic_size_t bytes_read_;
    static std::atomic_size_t bytes_written_;
};

}   // namespace net
}   // namespace chat

#endif  // _chat_network_util_statistics_hpp
