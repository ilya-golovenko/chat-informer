//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <crypto/manager.hpp>
#include <crypto/md5/computer.hpp>
#include <crypto/pkcs7/padding.hpp>

// STL headers
#include <stdexcept>
#include <cstring>


namespace chat
{
namespace crypto
{

manager::manager(std::string const& pass)
{
    set_passphrase(pass);
}

manager::manager(boost::uuids::uuid const& key)
{
    set_uuid_key(key);
}

void manager::set_passphrase(std::string const& pass)
{
    md5::computer md5_computer(pass.data(), pass.size());
    md5::digest md5_digest = md5_computer.get_digest();

    unsigned char key[aes::cipher::key_length];

    md5_digest.copy(key);
    cipher_.set_key(key);
}

void manager::set_uuid_key(boost::uuids::uuid const& key)
{
    cipher_.set_key(key.data);
}

void manager::encrypt(buffer_type const& input, buffer_type& output)
{
    if(!input.empty())
    {
        output.resize(input.size() + aes::cipher::block_size);
        output.resize(encrypt(input.data(), input.size(), output.data()));
    }
    else
    {
        output.clear();
    }
}

void manager::decrypt(buffer_type const& input, buffer_type& output)
{
    if(!input.empty())
    {
        output.resize(input.size());
        output.resize(decrypt(input.data(), input.size(), output.data()));
    }
    else
    {
        output.clear();
    }
}

std::size_t manager::encrypt(unsigned char const* input, std::size_t size, unsigned char* output)
{
    unsigned char const tail_size = size % aes::cipher::block_size;
    std::size_t const blocks_num = size / aes::cipher::block_size;

    size = (blocks_num + 1) * aes::cipher::block_size;

    for(std::size_t i = 0; i < blocks_num; ++i)
    {
        cipher_.encrypt_block(input, output);

        input += aes::cipher::block_size;
        output += aes::cipher::block_size;
    }

    unsigned char last[aes::cipher::block_size];
    std::memcpy(last, input, tail_size);

    add_pkcs7_padding(last, tail_size);
    cipher_.encrypt_block(last, output);

    return size;
}

std::size_t manager::decrypt(unsigned char const* input, std::size_t size, unsigned char* output)
{
    if(size % aes::cipher::block_size != 0u)
        throw std::runtime_error("data size is not a multiple of the block size");

    std::size_t const blocks_num = size / aes::cipher::block_size;

    for(std::size_t i = 0; i < blocks_num - 1; ++i)
    {
        cipher_.decrypt_block(input, output);

        input += aes::cipher::block_size;
        output += aes::cipher::block_size;
    }

    cipher_.decrypt_block(input, output);
    size -= remove_pkcs7_padding(output);

    return size;
}

void manager::add_pkcs7_padding(unsigned char* block, unsigned char data_size)
{
    pkcs7::add_padding(block, aes::cipher::block_size, data_size);
}

unsigned char manager::remove_pkcs7_padding(unsigned char const* block)
{
    return pkcs7::remove_padding(block, aes::cipher::block_size);
}

}   // namespace crypto
}   // namespace chat
