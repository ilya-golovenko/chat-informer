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


class CHtmlElement;

class CHtmlForm
{
public:
    CHtmlForm(CComPtr<IHTMLFormElement>& pHtmlFormElement);

    CHtmlForm(CHtmlForm const&) = default;
    CHtmlForm& operator=(CHtmlForm const&) = default;

    CHtmlElement GetElement(WTL::CString const& strName);

    void Submit();

private:
    ATL::CComPtr<IHTMLFormElement> m_spHtmlFormElement;
};
