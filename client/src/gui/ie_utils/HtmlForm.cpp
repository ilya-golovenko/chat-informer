//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/ie_utils/HtmlForm.h>
#include <informer/ie_utils/HtmlElement.h>

// STL headers
#include <stdexcept>


CHtmlForm::CHtmlForm(ATL::CComPtr<IHTMLFormElement>& spHtmlFormElement) :
    m_spHtmlFormElement(spHtmlFormElement)
{
}

CHtmlElement CHtmlForm::GetElement(CString const& strName)
{
    ATL::CComVariant vtName(strName);
    ATL::CComVariant vtIndex(0L, VT_I4);
    ATL::CComPtr<IDispatch> spHtmlDispatch;
    ATL::CComQIPtr<IHTMLElement> spHtmlElement;

    HRESULT hRet = m_spHtmlFormElement->item(vtName, vtIndex, &spHtmlDispatch);

    if(SUCCEEDED(hRet))
        spHtmlElement = spHtmlDispatch;

    if(FAILED(hRet) || !spHtmlElement)
        throw std::runtime_error("Cannot get HTML element");

    return CHtmlElement(spHtmlElement);
}

void CHtmlForm::Submit()
{
    HRESULT hRet = m_spHtmlFormElement->submit();

    if(FAILED(hRet))
        throw std::runtime_error("Cannot submit HTML form");
}
