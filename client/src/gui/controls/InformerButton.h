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
    #error InformerButton.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/controls/InformerControl.h>


template
<
    class T,
    class TBase = CButton,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CInformerButtonImpl :
    public CInformerControlImpl<T, TBase, TWinTraits>
{
public:
    enum
    {
        ITEM_NORMAL     = 0,
        ITEM_DISABLED   = 1,
        ITEM_HOVERED    = 2,
        ITEM_SELECTED   = 4,
        ITEM_FOCUSED    = 8
    };

public:
    static UINT const drawTextFormat = DT_CENTER | DT_VCENTER
        | DT_SINGLELINE | DT_EXPANDTABS | DT_NOPREFIX;

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    int GetItemState() const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        int itemState = ITEM_NORMAL;

        if(IsMouseOver())
            itemState |= ITEM_HOVERED;

        if(!IsWindowEnabled())
            itemState |= ITEM_DISABLED;

        int state = GetState();

        if(state & BST_PUSHED)
            itemState |= ITEM_SELECTED;

        if(state & BST_FOCUS)
            itemState |= ITEM_FOCUSED;

        return itemState;
    }

    // Overrideables

    void DrawItem(CDCHandle dc, CRect& rcItem, int itemState)
    {
        int pen;
        int brush;
        int textColor;

        if(itemState & ITEM_SELECTED)
        {
            pen = PEN_SELECTED;
            brush = BRUSH_SELECTED;
            textColor = COLOR_TEXT_SELECTED;
        }
        else if(itemState & ITEM_DISABLED)
        {
            pen = PEN_DISABLED;
            brush = BRUSH_DARK;
            textColor = COLOR_TEXT_DISABLED;
        }
        else if(itemState & ITEM_HOVERED)
        {
            pen = PEN_HOVERED;
            brush = BRUSH_HOVERED;
            textColor = COLOR_TEXT_HOVERED;
        }
        else
        {
            pen = PEN_NORMAL;
            brush = BRUSH_LIGHT;
            textColor = COLOR_TEXT_NORMAL;
        }

        if(itemState & ITEM_FOCUSED)
            pen = PEN_SELECTED;

        DrawRoundRect(dc, rcItem, pen, brush);

        int nLength = GetWindowTextLength();

        if(nLength > 0)
        {
            CString strText;

            LPWSTR lpBuffer = strText.GetBufferSetLength(nLength);

            if(NULL != lpBuffer)
            {
                GetWindowText(lpBuffer, nLength + 1);

                dc.SetBkMode(TRANSPARENT);
                dc.SelectFont(m_drawManager.Font(FONT_MAIN));
                dc.SetTextColor(m_drawManager.Color(textColor));
                dc.DrawText(lpBuffer, nLength, rcItem, drawTextFormat);
            }
        }
    }

    void DrawBackground(CDCHandle dc, CRect const& rcClient)
    {
        DrawFillRect(dc, rcClient, BRUSH_DARK);
    }

    void HandleMouseEnter(CPoint /*pt*/)
    {
        Invalidate();
        UpdateWindow();
    }

    void HandleMouseLeave()
    {
        Invalidate();
        UpdateWindow();
    }

    // Implementation

    CInformerButtonImpl()
    {
        EnableDoubleBuffering();
    }

    void DrawControl(CDCHandle dc, CRect& /*rc*/)
    {
        CRect rcClient;
        GetClientRect(rcClient);

        T* pT = static_cast<T*>(this);
        int itemState = GetItemState();

        pT->DrawBackground(dc, rcClient);
        pT->DrawItem(dc, rcClient, itemState);
    }
};

class CInformerButton :
    public CInformerButtonImpl<CInformerButton>
{
public:
    DECLARE_WND_SUPERCLASS(L"InformerButton", GetWndClassName())
};
