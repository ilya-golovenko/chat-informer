//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/common/Application.h>

// ATL/WTL headers
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>


CApplication _Application;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR lpszCmdLine, int nCmdShow)
{
    if(_Application.IsPreviousInstanceRunning())
        return _Application.ActivatePreviousInstance();

    HRESULT hRes = ::CoInitialize(NULL);
    ATLVERIFY(SUCCEEDED(hRes));

    // Initialize RichEdit dynamic library
    LPCWSTR lpszLibraryName = WTL::CRichEditCtrl::GetLibraryName();
    HINSTANCE hInstRich = ::LoadLibrary(lpszLibraryName);
    ATLVERIFY(NULL != hInstRich);

    // Initialize Common Controls library
    BOOL fRes = WTL::AtlInitCommonControls(ICC_BAR_CLASSES);
    ATLVERIFY(TRUE == fRes);

    // This resolves ATL window thunking problem when
    // Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    int nRet = _Application.Run(hInstance, lpszCmdLine, nCmdShow);

    ::FreeLibrary(hInstRich);
    ::CoUninitialize();

    return nRet;
}
