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
    #error InformerStaticText.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/controls/InformerFrame.h>


template
<
    class T,
    class TBase = CStatic,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CInformerStaticTextImpl :
    public CInformerFrameImpl<T, TBase, TWinTraits>
{
public:
    static UINT const drawTextFormat = DT_CENTER | DT_EXPANDTABS | DT_WORDBREAK;

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // CInformerFrame

    void DrawItem(CDCHandle dc, CRect& rcItem)
    {
        DrawFillRect(dc, rcItem, BRUSH_DARK);
        DrawRoundRect(dc, rcItem, PEN_STATIC, BRUSH_INVALID);

        int nLength = GetWindowTextLength();

        if(nLength > 0)
        {
            CString strText;

            LPWSTR lpBuffer = strText.GetBufferSetLength(nLength);

            if(NULL != lpBuffer)
            {
                GetWindowText(lpBuffer, nLength + 1);

                rcItem.DeflateRect(6, 6);
                CRect rcText(rcItem);

                dc.SetBkMode(TRANSPARENT);
                dc.SelectFont(m_drawManager.Font(FONT_MAIN));
                dc.SetTextColor(m_drawManager.Color(COLOR_TEXT_NORMAL));

                dc.DrawText(lpBuffer, nLength, rcText, drawTextFormat | DT_CALCRECT);

                rcText.OffsetRect((rcItem.Size() - rcText.Size()) / 2);
                dc.DrawText(lpBuffer, nLength, rcText, drawTextFormat);
            }
        }
    }
};

class CInformerStaticText :
    public CInformerStaticTextImpl<CInformerStaticText>
{
public:
    DECLARE_WND_SUPERCLASS(L"InformerStaticText", GetWndClassName())
};
