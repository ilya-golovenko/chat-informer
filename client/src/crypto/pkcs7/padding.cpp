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


namespace chat
{
namespace crypto
{
namespace pkcs7
{

void add_padding(unsigned char* block, unsigned char block_size, unsigned char data_size)
{
    if(data_size > block_size)
        throw std::runtime_error("invalid data size");

    unsigned char padding = block_size - data_size;

    if(0 == padding)
        padding = block_size;

    std::memset(block + data_size, padding, padding);
}

unsigned char remove_padding(unsigned char const* block, unsigned char block_size)
{
    unsigned char padding = block[block_size - 1];
    unsigned char size = block_size - padding;

    if(0 == padding || padding > block_size)
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
