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

class CHtmlElement
{
public:
    CHtmlElement(ATL::CComPtr<IHTMLElement>& spHtmlElement);

    CHtmlElement(CHtmlElement const&) = default;
    CHtmlElement& operator=(CHtmlElement const&) = default;

    CHtmlForm AsForm();

    void SetText(WTL::CString const& strText);

private:
    ATL::CComPtr<IHTMLElement> m_spHtmlElement;
};
