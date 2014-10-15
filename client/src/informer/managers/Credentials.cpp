//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2012, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/Credentials.h>
#include <informer/common/Config.h>
#include <network/util/base64.hpp>
#include <crypto/common.hpp>

// MISSIO headers
#include <missio/unicode/convert.hpp>


bool CCredentials::IsBad() const
{
    return cfg::chat::bad_credentials;
}

bool CCredentials::IsEmpty() const
{
    std::string const nickname = cfg::chat::nickname;
    std::string const password = cfg::chat::password;

    return nickname.empty() || password.empty();
}

std::wstring CCredentials::GetNickName() const
{
    return missio::unicode::to_wide_string(GetNickNameUTF8());
}

std::wstring CCredentials::GetPassword() const
{
    return missio::unicode::to_wide_string(GetPasswordUTF8());
}

std::string CCredentials::GetNickNameUTF8() const
{
    return cfg::chat::nickname;
}

std::string CCredentials::GetPasswordUTF8() const
{
    std::string password = cfg::chat::password;

    if(!password.empty())
    {
        password = net::util::base64::decode(password);
        password = crypto::decrypt(password, chat::crypto_key);
    }

    return password;
}

void CCredentials::Reset()
{
    cfg::chat::nickname.erase();
    cfg::chat::password.erase();

    cfg::chat::bad_credentials.erase();
}

bool CCredentials::Update(std::wstring const& nickname, std::wstring const& password)
{
    std::wstring const old_nickname = GetNickName();
    std::wstring const old_password = GetPassword();

    if(nickname != old_nickname || password != old_password)
    {
        std::string utf8_nickname = missio::unicode::to_utf8_string(nickname);
        std::string utf8_password = missio::unicode::to_utf8_string(password);

        if(!utf8_password.empty())
        {
            utf8_password = crypto::encrypt(utf8_password, chat::crypto_key);
            utf8_password = net::util::base64::encode(utf8_password);
        }

        cfg::chat::bad_credentials = false;

        cfg::chat::nickname = utf8_nickname;
        cfg::chat::password = utf8_password;

        return true;
    }

    return false;
}
