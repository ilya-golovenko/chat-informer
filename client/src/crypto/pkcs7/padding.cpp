//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <crypto/pkcs7/padding.hpp>

// STL headers
#include <stdexcept>
#include <cstring>


namespace chat
{
namespace crypto
{
namespace pkcs7
{

void add_padding(unsigned char* block, unsigned char block_size, unsigned char data_size)
{
    if(0u == block_size)
        throw std::runtime_error("invalid block size");

    if(data_size > block_size)
        throw std::runtime_error("invalid data size");

    unsigned char const padding = block_size - data_size % block_size;

    std::memset(block + data_size, padding, padding);
}

unsigned char remove_padding(unsigned char const* block, unsigned char block_size)
{
    if(0u == block_size)
        throw std::runtime_error("invalid block size");

    unsigned char const padding = block[block_size - 1];
    unsigned char const size = block_size - padding;

    if(padding > block_size)
        throw std::runtime_error("invalid PKCS#7 padding");

    for(unsigned char i = size; i < block_size - 1; ++i)
    {
        if(block[i] != padding)
            throw std::runtime_error("invalid PKCS#7 padding");
    }

    return padding;
}

}   // namespace pkcs7
}   // namespace crypto
}   // namespace chat
