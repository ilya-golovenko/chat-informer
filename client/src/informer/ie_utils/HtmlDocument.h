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


class CHtmlForm;
class CHtmlElement;

class CHtmlDocument
{
public:
    CHtmlDocument(ATL::CComPtr<IHTMLDocument3>& spHtmlDocument3);

    CHtmlDocument(CHtmlDocument const&) = default;
    CHtmlDocument& operator=(CHtmlDocument const&) = default;

    WTL::CString GetURL();
    WTL::CString GetDomain();

    CHtmlElement GetBody();
    CHtmlForm GetForm(WTL::CString const& strFormName);
    CHtmlElement GetElementById(WTL::CString const& strName);
    CHtmlElement GetElementByTagName(WTL::CString const& strTagName);

private:
    ATL::CComPtr<IHTMLDocument3> m_spHtmlDocument3;
};
