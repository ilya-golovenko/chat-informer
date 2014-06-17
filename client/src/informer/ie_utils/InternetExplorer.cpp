//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/ie_utils/InternetExplorer.h>
#include <informer/ie_utils/HtmlDocument.h>

// STL headers
#include <stdexcept>


CInternetExplorer::CInternetExplorer()
{
}

CInternetExplorer::~CInternetExplorer()
{
}

void CInternetExplorer::Start()
{
    HRESULT hRet = m_spWebBrowser2.CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_SERVER);

    if(FAILED(hRet) || !m_spWebBrowser2)
        throw std::runtime_error("Cannot start Internet Explorer server");
}

void CInternetExplorer::Quit()
{
    if(m_spWebBrowser2)
    {
        HRESULT hRet = m_spWebBrowser2->Quit();

        if(FAILED(hRet))
            throw std::runtime_error("Cannot quit Internet Explorer server");

        m_spWebBrowser2.Release();
    }
}

void CInternetExplorer::Navigate(WTL::CString const& strUrl)
{
    if(m_spWebBrowser2)
    {
        ATL::CComVariant vtEmpty;
        ATL::CComBSTR bstrUrl(strUrl);

        HRESULT hRet = m_spWebBrowser2->Navigate(bstrUrl, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);

        if(FAILED(hRet))
            throw std::runtime_error("Cannot navigate to specified URL");
    }
    else
    {
        throw std::runtime_error("Internet Explorer server is not initialized");
    }
}

void CInternetExplorer::WaitReady(DWORD dwTimeout)
{
    if(m_spWebBrowser2)
    {
        if(!WaitWhileBusy(dwTimeout))
            throw std::runtime_error("Waiting for ready-state failed");

        if(!WaitWhileNotReady(dwTimeout))
            throw std::runtime_error("Waiting for ready-state failed");

        ::Sleep(2000);
    }
    else
    {
        throw std::runtime_error("Internet Explorer server is not initialized");
    }
}

BOOL CInternetExplorer::IsVisible()
{
    if(m_spWebBrowser2)
    {
        VARIANT_BOOL vtVisible;

        HRESULT hRet = m_spWebBrowser2->get_Visible(&vtVisible);

        if(FAILED(hRet))
            throw std::runtime_error("Cannot get visible status");

        return (VARIANT_TRUE == vtVisible);
    }

    throw std::runtime_error("Internet Explorer server is not initialized");
}

void CInternetExplorer::SetVisible(BOOL fVisible)
{
    if(m_spWebBrowser2)
    {
        if(TRUE == fVisible)
        {
            BOOL fRet = ::ShowWindowAsync(GetWindowHandle(), SW_SHOWMAXIMIZED);

            if(FALSE == fRet)
                throw std::runtime_error("Cannot set visible status");
        }
        else
        {
            HRESULT hRet = m_spWebBrowser2->put_Visible(VARIANT_FALSE);

            if(FAILED(hRet))
                throw std::runtime_error("Cannot set visible status");
        }
    }
    else
    {
        throw std::runtime_error("Internet Explorer server is not initialized");
    }
}

HWND CInternetExplorer::GetWindowHandle()
{
    if(m_spWebBrowser2)
    {
        HWND hWnd = NULL;

        HRESULT hRet = m_spWebBrowser2->get_HWND(static_cast<long*>(&hWnd));

        if(FAILED(hRet) || NULL == hWnd)
            throw std::runtime_error("Cannot get window handle");

        return hWnd;
    }

    throw std::runtime_error("Internet Explorer server is not initialized");
}

CHtmlDocument CInternetExplorer::GetDocument()
{
    if(m_spWebBrowser2)
    {
        ATL::CComPtr<IDispatch> spHtmlDispatch;
        ATL::CComQIPtr<IHTMLDocument3> spHtmlDocument3;

        HRESULT hRet = m_spWebBrowser2->get_Document(&spHtmlDispatch);

        if(SUCCEEDED(hRet))
            spHtmlDocument3 = spHtmlDispatch;

        if(FAILED(hRet) || !spHtmlDocument3)
            throw std::runtime_error("Cannot get HTML document");

        return CHtmlDocument(spHtmlDocument3);
    }

    throw std::runtime_error("Internet Explorer server is not initialized");
}

BOOL CInternetExplorer::WaitWhileBusy(DWORD dwTimeout)
{
    VARIANT_BOOL vtBusy;
    DWORD dwStart = ::GetTickCount();

    do
    {
        ::Sleep(100);

        if(INFINITE != dwTimeout)
        {
            if(::GetTickCount() - dwStart > dwTimeout)
                return FALSE;
        }

        if(FAILED(m_spWebBrowser2->get_Busy(&vtBusy)))
            return FALSE;
    }
    while(VARIANT_TRUE == vtBusy);

    return TRUE;
}

BOOL CInternetExplorer::WaitWhileNotReady(DWORD dwTimeout)
{
    READYSTATE readyState;
    DWORD dwStart = ::GetTickCount();

    do
    {
        ::Sleep(100);

        if(INFINITE != dwTimeout)
        {
            if(::GetTickCount() - dwStart > dwTimeout)
                return FALSE;
        }

        if(FAILED(m_spWebBrowser2->get_ReadyState(&readyState)))
            return FALSE;
    }
    while(READYSTATE_COMPLETE != readyState);

    return TRUE;
}
