//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/credentials.hpp>
#include <network/util/base64.hpp>
#include <crypto/common.hpp>

// MISSIO headers
#include <missio/unicode/convert.hpp>


namespace chat
{

credentials::credentials(std::wstring const& nickname, std::wstring const& password) :
    credentials_(make_credentials(nickname, password))
{
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
    return net::base64::encode(nickname + ':' + crypto::calculate_md5(password));
}

std::string credentials::make_credentials(std::wstring const& nickname, std::wstring const& password)
{
    return make_credentials(missio::unicode::to_utf8_string(nickname), missio::unicode::to_utf8_string(password));
}

}   // namespace chat
