//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
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
    ~CHtmlElement();

    CHtmlElement(CHtmlElement const& other);
    CHtmlElement& operator=(CHtmlElement const& other);

    CHtmlForm AsForm();

    void SetText(WTL::CString const& strText);

private:
    ATL::CComPtr<IHTMLElement> m_spHtmlElement;
};
