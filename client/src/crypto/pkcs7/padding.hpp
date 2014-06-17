//---------------------------------------------------------------------------
//
//    This file is part of missio project
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _crypto_pkcs7_padding_hpp
#define _crypto_pkcs7_padding_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)


namespace crypto
{
namespace pkcs7
{

void add_padding(unsigned char* block, unsigned char block_size, unsigned char data_size);
unsigned char remove_padding(unsigned char const* block, unsigned char block_size);

}   // namespace pkcs7
}   // namespace crypto

#endif  // _crypto_pkcs7_padding_hpp
