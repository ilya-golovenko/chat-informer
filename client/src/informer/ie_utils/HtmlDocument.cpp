//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/ie_utils/HtmlDocument.h>
#include <informer/ie_utils/HtmlElement.h>
#include <informer/ie_utils/HtmlForm.h>

// STL headers
#include <stdexcept>


CHtmlDocument::CHtmlDocument(ATL::CComPtr<IHTMLDocument3>& spHtmlDocument3) :
    m_spHtmlDocument3(spHtmlDocument3)
{
}

WTL::CString CHtmlDocument::GetURL()
{
    ATL::CComQIPtr<IHTMLDocument2> spHtmlDocument2 = m_spHtmlDocument3;

    if(!spHtmlDocument2)
        throw std::runtime_error("Cannot get HTML document interface");

    ATL::CComBSTR bstrURL;
    HRESULT hRet = spHtmlDocument2->get_URL(&bstrURL);

    if(FAILED(hRet))
        throw std::runtime_error("Cannot get HTML document location");

    return WTL::CString(bstrURL);
}

WTL::CString CHtmlDocument::GetDomain()
{
    ATL::CComQIPtr<IHTMLDocument2> spHtmlDocument2 = m_spHtmlDocument3;

    if(!spHtmlDocument2)
        throw std::runtime_error("Cannot get HTML document interface");

    ATL::CComBSTR bstrDomain;
    HRESULT hRet = spHtmlDocument2->get_domain(&bstrDomain);

    if(FAILED(hRet))
        throw std::runtime_error("Cannot get HTML document domain");

    return WTL::CString(bstrDomain);
}

CHtmlElement CHtmlDocument::GetBody()
{
    CComQIPtr<IHTMLDocument2> spHtmlDocument2 = m_spHtmlDocument3;

    if(!spHtmlDocument2)
        throw std::runtime_error("Cannot get HTML document interface");

    ATL::CComPtr<IHTMLElement> spHtmlElement;
    HRESULT hRet = spHtmlDocument2->get_body(&spHtmlElement);

    if(FAILED(hRet) || !spHtmlElement)
        throw std::runtime_error("Cannot get HTML body");

    return CHtmlElement(spHtmlElement);
}

CHtmlForm CHtmlDocument::GetForm(WTL::CString const& strFormName)
{
    if(!strFormName.IsEmpty())
        return GetElementById(strFormName).AsForm();

    return GetElementByTagName(L"FORM").AsForm();
}

CHtmlElement CHtmlDocument::GetElementById(WTL::CString const& strName)
{
    ATL::CComBSTR bstrName(strName);
    ATL::CComPtr<IHTMLElement> spHtmlElement;

    HRESULT hRet = m_spHtmlDocument3->getElementById(bstrName, &spHtmlElement);

    if(FAILED(hRet) || !spHtmlElement)
        throw std::runtime_error("Cannot get HTML element");

    return CHtmlElement(spHtmlElement);
}

CHtmlElement CHtmlDocument::GetElementByTagName(WTL::CString const& strTagName)
{
    ATL::CComPtr<IDispatch> spHtmlDispatch;
    ATL::CComQIPtr<IHTMLElement> spHtmlElement;
    ATL::CComPtr<IHTMLElementCollection> spHtmlElements;

    ATL::CComBSTR bstrTagName(strTagName);

    HRESULT hRet = m_spHtmlDocument3->getElementsByTagName(bstrTagName, &spHtmlElements);

    if(SUCCEEDED(hRet))
    {
        ATL::CComVariant vtName(0L);
        ATL::CComVariant vtIndex;

        hRet = spHtmlElements->item(vtName, vtIndex, &spHtmlDispatch);

        if(SUCCEEDED(hRet))
            spHtmlElement = spHtmlDispatch;
    }

    if(FAILED(hRet) || !spHtmlElement)
        throw std::runtime_error("Cannot get HTML element");

    return CHtmlElement(spHtmlElement);
}
