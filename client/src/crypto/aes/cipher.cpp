//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <crypto/aes/cipher.hpp>


namespace chat
{
namespace crypto
{
namespace aes
{

cipher::cipher()
{
    int const key_bits = static_cast<int>(key_length << 3);
    int const block_bits = static_cast<int>(block_size << 3);

    encrypt_context_.SetParameters(key_bits, block_bits);
    decrypt_context_.SetParameters(key_bits, block_bits);
}

void cipher::set_key(unsigned char const key[key_length])
{
    encrypt_context_.StartEncryption(key);
    decrypt_context_.StartDecryption(key);
}

void cipher::encrypt_block(unsigned char const* input, unsigned char* output)
{
    encrypt_context_.EncryptBlock(input, output);
}

void cipher::decrypt_block(unsigned char const* input, unsigned char* output)
{
    decrypt_context_.DecryptBlock(input, output);
}

}   // namespace aes
}   // namespace crypto
}   // namespace chat
