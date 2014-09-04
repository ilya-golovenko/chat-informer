//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_crypto_pkcs7_padding_hpp
#define _chat_crypto_pkcs7_padding_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)


namespace chat
{
namespace crypto
{
namespace pkcs7
{

void add_padding(unsigned char* block, unsigned char block_size, unsigned char data_size);
unsigned char remove_padding(unsigned char const* block, unsigned char block_size);

}   // namespace pkcs7
}   // namespace crypto
}   // namespace chat

#endif  // _chat_crypto_pkcs7_padding_hpp
