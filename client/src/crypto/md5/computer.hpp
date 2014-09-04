//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_crypto_md5_computer_hpp
#define _chat_crypto_md5_computer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <crypto/md5/digest.hpp>

// MD5 headers
#include <crypto/impl/md5/md5.h>

// BOOST headers
#include <boost/noncopyable.hpp>

// STL headers
#include <cstddef>


namespace chat
{
namespace crypto
{
namespace md5
{

class computer : private boost::noncopyable
{
public:
    computer();

    computer(void const* data, std::size_t size);

    void append(void const* data, std::size_t size);

    digest get_digest();

private:
    md5_state_t state_;
    md5_byte_t digest_[16];
};

}   // namespace md5
}   // namespace crypto
}   // namespace chat

#endif  // _chat_crypto_md5_computer_hpp
