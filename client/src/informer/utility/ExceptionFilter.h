//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// STL headers
#include <string>

// Windows headers
#include <windows.h>


class CExceptionFilter
{
public:
    CExceptionFilter();
    ~CExceptionFilter();

    CExceptionFilter(CExceptionFilter const&) = delete;
    CExceptionFilter& operator=(CExceptionFilter const&) = delete;

    void Install();
    void Uninstall();

private:
    static LONG WINAPI UnhandledExceptionFilter(EXCEPTION_POINTERS* info);

    static std::wstring GetModuleName(LPCVOID address);
    static wchar_t const* GetExceptionString(DWORD exception);

private:
    LPTOP_LEVEL_EXCEPTION_FILTER m_oldExceptionFilter;
};
