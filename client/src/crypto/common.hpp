//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_crypto_common_hpp
#define _chat_crypto_common_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/uuid/uuid.hpp>

// STL headers
#include <string>


namespace chat
{
namespace crypto
{

class manager;

std::string encrypt(std::string const& data, std::string const& pass);
std::string decrypt(std::string const& data, std::string const& pass);

std::string encrypt(std::string const& data, boost::uuids::uuid const& key);
std::string decrypt(std::string const& data, boost::uuids::uuid const& key);

std::string encrypt(std::string const& data, crypto::manager& crypto_manager);
std::string decrypt(std::string const& data, crypto::manager& crypto_manager);

std::string calculate_md5(std::string const& data);

}   // namespace crypto
}   // namespace chat

#endif  // _chat_crypto_common_hpp
