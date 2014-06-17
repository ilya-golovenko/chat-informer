//---------------------------------------------------------------------------
//
//    This file is part of missio project
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <crypto/md5/computer.hpp>


namespace crypto
{
namespace md5
{

computer::computer()
{
    ::md5_init(&state_);
}

computer::computer(const void* data, std::size_t size)
{
    ::md5_init(&state_);
    append(data, size);
}

void computer::append(void const* data, std::size_t size)
{
    ::md5_append(&state_, static_cast<md5_byte_t const*>(data), size);
}

digest computer::get_digest()
{
    ::md5_finish(&state_, digest_);
    return digest(digest_);
}

}   // namespace md5
}   // namespace crypto
