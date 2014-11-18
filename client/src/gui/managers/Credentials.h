//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2012, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// STL headers
#include <string>


class CCredentials
{
public:
    CCredentials() = default;
    ~CCredentials() = default;

    bool IsBad() const;
    bool IsEmpty() const;

    std::wstring GetNickName() const;
    std::wstring GetPassword() const;

    std::string GetNickNameUTF8() const;
    std::string GetPasswordUTF8() const;

    void Reset();

    bool Update(std::wstring const& nickname, std::wstring const& password);
};
