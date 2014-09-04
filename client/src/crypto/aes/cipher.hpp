//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_crypto_aes_cipher_hpp
#define _chat_crypto_aes_cipher_hpp

// AES headers
#include <crypto/impl/aes/aes.h>

// BOOST headers
#include <boost/noncopyable.hpp>


namespace chat
{
namespace crypto
{
namespace aes
{

class cipher : private boost::noncopyable
{
public:
    static unsigned char const key_length = 16;
    static unsigned char const block_size = 16;

public:
    cipher();
    ~cipher();

    void set_key(unsigned char const key[key_length]);

    void encrypt_block(unsigned char const* input, unsigned char* output);
    void decrypt_block(unsigned char const* input, unsigned char* output);

private:
    AES encrypt_context_;
    AES decrypt_context_;
};

}   // namespace aes
}   // namespace crypto
}   // namespace chat

#endif  // _chat_crypto_aes_cipher_hpp
