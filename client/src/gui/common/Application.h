//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/utility/ExceptionFilter.h>
#include <informer/utility/SingleInstance.h>
#include <network/http/proxy_settings.hpp>

// Windows headers
#include <windows.h>

// ATL/WTL headers
#include <atlbase.h>
#include <atlwin.h>


class CApplication
{
public:
    CApplication();
    ~CApplication();

    CApplication(CApplication const&) = delete;
    CApplication& operator=(CApplication const&) = delete;

    BOOL IsPreviousInstanceRunning();
    int ActivatePreviousInstance();

    int Run(HINSTANCE hInstance, LPWSTR lpszCmdLine, int nCmdShow);

private:
    BOOL Initialize(HINSTANCE hInstance);
    void Configure();
    void Finalize();

    void SetupLogger();
    void SetupInstanceID();
    void SetupProxySettings();

    ATL::CWindow CreateMainDialog();

    void LoadConfiguration();
    void SaveConfiguration();

private:
    static void ShowErrorMessage(UINT stringID);
    
    net::http::proxy_settings GetUserProxySettings();
    net::http::proxy_settings GetSystemProxySettings();

private:
    CSingleInstance m_singleInstance;
    CExceptionFilter m_exceptionFilter;
};
