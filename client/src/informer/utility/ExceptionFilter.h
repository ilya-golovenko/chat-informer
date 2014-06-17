//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Windows headers
#include <windows.h>

// BOOST headers
#include <boost/noncopyable.hpp>

// STL headers
#include <string>


class CExceptionFilter :
    private boost::noncopyable
{
public:
    CExceptionFilter();
    ~CExceptionFilter();

    void Install();
    void Uninstall();

private:
    static LONG WINAPI UnhandledExceptionFilter(EXCEPTION_POINTERS* info);

    static std::wstring GetModuleName(LPCVOID address);
    static wchar_t const* GetExceptionString(DWORD exception);

private:
    LPTOP_LEVEL_EXCEPTION_FILTER m_oldExceptionFilter;
};
