//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// ATL/WTL headers
#include <atlbase.h>
#include <atlmisc.h>
#include <atlcomcli.h>


class CHtmlDocument;

class CInternetExplorer
{
public:
    CInternetExplorer() = default;
    ~CInternetExplorer() = default;

    CInternetExplorer(CInternetExplorer const&) = delete;
    CInternetExplorer& operator=(CInternetExplorer const&) = delete;

    void Start();
    void Quit();

    void Navigate(WTL::CString const& strUrl);
    void WaitReady(DWORD dwTimeout);

    BOOL IsVisible();
    void SetVisible(BOOL fVisible);

    HWND GetWindowHandle();
    CHtmlDocument GetDocument();

private:
    BOOL WaitWhileBusy(DWORD dwTimeout);
    BOOL WaitWhileNotReady(DWORD dwTimeout);

private:
    ATL::CComPtr<IWebBrowser2> m_spWebBrowser2;
};
