//---------------------------------------------------------------------------
//
//    This file is part of missio project
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <crypto/aes/cipher.hpp>


namespace crypto
{
namespace aes
{

cipher::cipher()
{
    int key_bits = static_cast<int>(key_length << 3);
    int block_bits = static_cast<int>(block_size << 3);

    encrypt_context_.SetParameters(key_bits, block_bits);
    decrypt_context_.SetParameters(key_bits, block_bits);
}

cipher::~cipher()
{
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
