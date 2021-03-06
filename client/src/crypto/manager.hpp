//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_crypto_manager_hpp
#define _chat_crypto_manager_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <crypto/aes/cipher.hpp>
#include <utility/raw_buffer.hpp>

// BOOST headers
#include <boost/uuid/uuid.hpp>


namespace chat
{
namespace crypto
{

class manager
{
public:
    typedef raw_buffer buffer_type;

public:
    manager() = default;
    ~manager() = default;

    explicit manager(std::string const& pass);
    explicit manager(boost::uuids::uuid const& key);

    manager(manager const&) = delete;
    manager& operator=(manager const&) = delete;

    void set_passphrase(std::string const& pass);
    void set_uuid_key(boost::uuids::uuid const& key);

    void encrypt(buffer_type const& input, buffer_type& output);
    void decrypt(buffer_type const& input, buffer_type& output);

private:
    std::size_t encrypt(unsigned char const* input, std::size_t size, unsigned char* output);
    std::size_t decrypt(unsigned char const* input, std::size_t size, unsigned char* output);

private:
    static void add_pkcs7_padding(unsigned char* block, unsigned char data_size);
    static unsigned char remove_pkcs7_padding(const unsigned char* block);

private:
    aes::cipher cipher_;
};

}   // namespace crypto
}   // namespace chat

#endif  // _chat_crypto_manager_hpp
