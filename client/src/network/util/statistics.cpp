//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/util/statistics.hpp>


namespace chat
{
namespace net
{

std::atomic_size_t statistics::bytes_read_ = ATOMIC_VAR_INIT(0u);
std::atomic_size_t statistics::bytes_written_ = ATOMIC_VAR_INIT(0u);

std::size_t statistics::get_bytes_read()
{
    return bytes_read_.load(std::memory_order_relaxed);
}

std::size_t statistics::get_bytes_written()
{
    return bytes_written_.load(std::memory_order_relaxed);
}

void statistics::add_bytes_read(std::size_t count)
{
    bytes_read_.fetch_add(count, std::memory_order_relaxed);
}

void statistics::add_bytes_written(std::size_t count)
{
    bytes_written_.fetch_add(count, std::memory_order_relaxed);
}

}   // namespace net
}   // namespace chat
