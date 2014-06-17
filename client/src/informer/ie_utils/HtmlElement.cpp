//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/ie_utils/HtmlElement.h>
#include <informer/ie_utils/HtmlForm.h>

// STL headers
#include <stdexcept>


CHtmlElement::CHtmlElement(ATL::CComPtr<IHTMLElement>& spHtmlElement) :
    m_spHtmlElement(spHtmlElement)
{
}

CHtmlElement::~CHtmlElement()
{
}

CHtmlElement::CHtmlElement(CHtmlElement const& other) :
    m_spHtmlElement(other.m_spHtmlElement)
{
}

CHtmlElement& CHtmlElement::operator=(CHtmlElement const& other)
{
    if(&other != this)
        m_spHtmlElement = other.m_spHtmlElement;
    return *this;
}

CHtmlForm CHtmlElement::AsForm()
{
    ATL::CComQIPtr<IHTMLFormElement> spHtmlFormElement = m_spHtmlElement;

    if(!spHtmlFormElement)
        throw std::runtime_error("Cannot get HTML form");

    return CHtmlForm(spHtmlFormElement);
}

void CHtmlElement::SetText(WTL::CString const& strText)
{
    ATL::CComBSTR bstrText(strText);

    HRESULT hRet = m_spHtmlElement->put_innerText(bstrText);

    if(FAILED(hRet))
        throw std::runtime_error("Cannot set HTML element text");
}
