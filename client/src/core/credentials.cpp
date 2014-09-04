//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/credentials.hpp>
#include <net/util/base64.hpp>
#include <crypto/common.hpp>

// MISSIO headers
#include <missio/unicode/convert.hpp>

// STL headers
#include <utility>


namespace chat
{

credentials::credentials()
{
}

credentials::~credentials()
{
}

credentials::credentials(std::wstring const& nickname, std::wstring const& password) :
    credentials_(make_credentials(nickname, password))
{
}

credentials::credentials(credentials&& other) :
    credentials_(std::move(other.credentials_))
{
}

credentials& credentials::operator=(credentials&& other)
{
    if(&other != this)
        credentials_ = std::move(other.credentials_);
    return *this;
}

credentials::credentials(credentials const& other) :
    credentials_(other.credentials_)
{
}

credentials& credentials::operator=(credentials const& other)
{
    if(&other != this)
        credentials_ = other.credentials_;
    return *this;
}

std::string credentials::to_string() const
{
    return credentials_;
}

std::wstring credentials::to_wstring() const
{
    return missio::unicode::to_wide_string(to_string());
}

std::string credentials::make_credentials(std::string const& nickname, std::string const& password)
{
    return net::util::base64::encode(nickname + ':' + crypto::calculate_md5(password));
}

std::string credentials::make_credentials(std::wstring const& nickname, std::wstring const& password)
{
    return make_credentials(missio::unicode::to_utf8_string(nickname), missio::unicode::to_utf8_string(password));
}

}   // namespace chat
