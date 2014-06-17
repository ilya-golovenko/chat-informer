//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// ATL/WTL headers
#include <atlwin.h>

#ifndef __cplusplus
    #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLWIN_H__
    #error InformerHtmlCtrl.h requires atlwin.h to be included first
#endif

#include <informer/utility/InformerHelper.h>
#include <informer/utility/MouseTrackHelper.h>
#include <informer/utility/ShellExecuteHelper.h>


template
<
    class T,
    class TBase = CRichEditCtrl,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CInformerHtmlCtrlImpl :
    public CWindowImpl<T, TBase, TWinTraits>,
    public CMouseTrackHelper<T>,
    public CInformerHelper
{
public:
    enum { MARGIN = 16 };

public:
    struct RTFSTREAM
    {
        LPCWSTR lpszString;
        DWORD dwPosition;
    };

    struct CCharFormat : CHARFORMAT2
    {
        CCharFormat()
        {
            ::ZeroMemory(this, sizeof(CHARFORMAT2));
            cbSize = sizeof(CHARFORMAT2);
        }
    };

    struct CCursorInfo : CURSORINFO
    {
        CCursorInfo()
        {
            ::ZeroMemory(this, sizeof(CURSORINFO));
            cbSize = sizeof(CURSORINFO);
        }
    };

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    BOOL SubclassWindow(HWND hWnd)
    {
        ATLASSERT(m_hWnd == NULL);
        ATLASSERT(::IsWindow(hWnd));

        BOOL bRet = CWindowImpl<T, TBase, TWinTraits>::SubclassWindow(hWnd);

        if(bRet)
        {
            T* pT = static_cast<T*>(this);
            pT->Init();
        }

        return bRet;
    }
/*
    BOOL LoadHtml(CString const& strHtmlData)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        std::string html_data;// = strHtmlData;

        std::string rtf_data = m_converter.convert(html_data);
        util::rtf_color_t rtf_color = m_converter.get_background_color();
        SetBackgroundColor(RGB(rtf_color.red, rtf_color.green, rtf_color.blue));

        RTFSTREAM rtfStream;

        //rtfStream.lpszString = rtf_data.c_str();
        rtfStream.dwPosition = 0;

        EDITSTREAM editStream;

        editStream.dwCookie = reinterpret_cast<DWORD_PTR>(&rtfStream);
        editStream.pfnCallback = &StreamReadCallback;
        editStream.dwError = 0;

        StreamIn(SF_RTF, editStream);
        return !editStream.dwError;
    }
*/
    // Message map and handlers

    BEGIN_MSG_MAP(CInformerHtmlCtrlImpl)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MSG_WM_SETFOCUS(OnSetFocus)
        MSG_WM_KILLFOCUS(OnKillFocus)
        MSG_WM_SETCURSOR(OnSetCursor)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(EN_MSGFILTER, OnMsgNotify)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(EN_REQUESTRESIZE, OnRequestResize)
        CHAIN_MSG_MAP(CMouseTrackHelper<T>)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
    {
        LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);

        T* pT = static_cast<T*>(this);
        pT->Init();

        return lRet;
    }

    void OnSetFocus(HWND /*hWndOld*/)
    {
        SetFocus();
        HideCaret();
    }

    void OnKillFocus(HWND /*hWndFocus*/)
    {
/*
        if(m_old_link)
        {
            SetHyperlinkColor(*m_old_link, false);
            m_old_link = util::optional_rtf_link_t();
        }
*/
    }

    BOOL OnSetCursor(HWND /*hWnd*/, UINT nHitTest, UINT /*message*/)
    {
        return FALSE;
//        return (m_old_link && HTCLIENT == nHitTest ? TRUE : FALSE);
    }

    LRESULT OnMsgNotify(LPNMHDR pnmh)
    {
/*
        MSGFILTER* msgFilter = reinterpret_cast<MSGFILTER*>(pnmh);

        CPoint pt(msgFilter->lParam);
        int charPos = CharFromPos(pt);

        bool is_link_changed = false;
        util::optional_rtf_link_t this_link;

        if(m_old_link)
        {
            if(m_old_link->range.contains(charPos))
                this_link = m_old_link;
        }

        if(!this_link)
        {
            this_link = m_converter.find_link(charPos, charPos);
            is_link_changed = this_link != m_old_link;
        }

        switch(msgFilter->msg)
        {
            case WM_MOUSEMOVE:
                if(is_link_changed)
                {
                    if(this_link)
                        ::SetCursor(AtlLoadSysCursor(IDC_HAND));
                    else
                        ::SetCursor(AtlLoadSysCursor(IDC_ARROW));

                    if(this_link)
                        SetHyperlinkColor(*this_link, true);

                    if(m_old_link)
                        SetHyperlinkColor(*m_old_link, false);
                }
                break;

            case WM_LBUTTONDOWN:
                if(this_link)
                    NavigateToHyperlinkUrl(*this_link);
                break;
        }

        if(is_link_changed)
            m_old_link = this_link;
*/
        return TRUE;
    }

    LRESULT OnRequestResize(LPNMHDR pnmh)
    {
        REQRESIZE* reqResize = reinterpret_cast<REQRESIZE*>(pnmh);

        CWindow wndParent = GetParent();
        CRect rcRequired(reqResize->rc);

        wndParent.ResizeClient(
            rcRequired.Width() + MARGIN,
            rcRequired.Height() + MARGIN);

        return TRUE;
    }

    // CMouseTrackHelper

    void HandleMouseLeave()
    {
/*
        if(m_old_link)
        {
            SetHyperlinkColor(*m_old_link, false);
            m_old_link = util::optional_rtf_link_t();
        }
*/
    }

    // Implementation

    void Init()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT((GetStyle() & ES_MULTILINE) == ES_MULTILINE);
        SetEventMask(ENM_REQUESTRESIZE | ENM_SELCHANGE | ENM_MOUSEEVENTS);
    }

    static DWORD CALLBACK StreamReadCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
    {
        ATLASSERT(dwCookie != 0);
        ATLASSERT(pcb != NULL);

        RTFSTREAM* lpRtfStream = reinterpret_cast<RTFSTREAM*>(dwCookie);

        LPCWSTR lpszString = lpRtfStream->lpszString;
        lpszString += lpRtfStream->dwPosition;

        LONG length = ::lstrlen(lpszString);

        if(cb > length)
            cb = length;

        ::CopyMemory(pbBuff, lpszString, cb);
        lpRtfStream->dwPosition += cb;

        *pcb = cb;
        return 0;
    }
/*
    void SetHyperlinkColor(util::rtf_link_t const& link, bool hovered)
    {
        util::rtf_color_t rtf_color = m_converter.get_hyperlink_color(hovered);
        COLORREF color = RGB(rtf_color.red, rtf_color.green, rtf_color.blue);

        CCharFormat charFormat;
        charFormat.dwMask = CFM_COLOR;
        charFormat.crTextColor = color;

        SetSel(link.range.begin, link.range.end);
        SetSelectionCharFormat(charFormat);

        SetSel(0, 0);
    }

    void NavigateToHyperlinkUrl(util::rtf_link_t const& link)
    {
        CShellExecuteHelper(link.url).Start();
    }
*/
private:
    //util::html2rtf_converter m_converter;
    //util::optional_rtf_link_t m_old_link;
};

class CInformerHtmlCtrl :
    public CInformerHtmlCtrlImpl<CInformerHtmlCtrl>
{
public:
    DECLARE_WND_SUPERCLASS(L"InformerHtmlCtrl", GetWndClassName())
};
