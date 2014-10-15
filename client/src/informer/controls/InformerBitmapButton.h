//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// ATL/WTL headers
#include <atlwin.h>

#ifndef __cplusplus
    #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLWIN_H__
    #error InformerBitmapButton.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/controls/InformerButton.h>


template
<
    class T,
    class TBase = CButton,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CInformerBitmapButtonImpl :
    public CInformerButtonImpl<T, TBase, TWinTraits>
{
public:
    // Members
    CToolTipCtrl m_ctrlToolTip;
    std::uniqure_ptr<WCHAR[]> m_strTip;

public:
    enum
    {
        NORMAL_IMAGE,
        PUSHED_IMAGE,
        HOVERED_IMAGE,
        DISABLED_IMAGE,

        IMAGE_COUNT
    };

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    void SetImages(int normal, int pushed = -1, int hovered = -1, int disabled = -1)
    {
        m_images[NORMAL_IMAGE] = normal;
        m_images[PUSHED_IMAGE] = pushed;
        m_images[HOVERED_IMAGE] = hovered;
        m_images[DISABLED_IMAGE] = disabled;
    }

    bool SetToolTipText(LPCWSTR lpszText)
    {
        if(NULL == lpszText)
        {
            m_strTip.reset();

            if(m_ctrlToolTip.IsWindow())
                m_ctrlToolTip.Activate(FALSE);

            return true;
        }

        int length = lstrlen(lpszText) + 1;
        m_strTip.reset(new WCHAR[length]);
        lstrcpy(m_strTip.get(), lpszText);

        if(!m_ctrlToolTip.IsWindow())
        {
            m_ctrlToolTip.Create(m_hWnd, NULL, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, WS_EX_TOPMOST);

            ATLASSERT(m_ctrlToolTip.IsWindow());

            m_ctrlToolTip.Activate(TRUE);
            m_ctrlToolTip.SetTipBkColor(m_drawManager.Color(COLOR_BACK_LIGHT));
            m_ctrlToolTip.SetTipTextColor(m_drawManager.Color(COLOR_TEXT_NORMAL));
        }

        CToolInfo toolInfo(TTF_SUBCLASS, m_hWnd, 0, 0, m_strTip.get());

        m_ctrlToolTip.AddTool(toolInfo);

        return true;
    }

    // Message map and handlers

    BEGIN_MSG_MAP(CInformerBitmapButtonImpl)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        CHAIN_MSG_MAP(CInformerButtonImpl)
    END_MSG_MAP()

    void OnDestroy()
    {
        SetMsgHandled(FALSE);

        if(m_ctrlToolTip.IsWindow())
            m_ctrlToolTip.DestroyWindow();
    }

    void OnLButtonDown(UINT /*nFlags*/, CPoint /*pt*/)
    {
        SetMsgHandled(FALSE);

        if(m_ctrlToolTip.IsWindow())
        {
            m_ctrlToolTip.Activate(FALSE);
            m_ctrlToolTip.Activate(TRUE);
        }
    }

    // CInformerButton

    void DrawItem(CDCHandle dc, CRect& rcItem, int itemState)
    {
        int pen;
        int brush;

        int imageIndex;
        int imageState;

        if(itemState & ITEM_SELECTED)
        {
            pen = PEN_SELECTED;
            brush = BRUSH_SELECTED;

            imageIndex = PUSHED_IMAGE;
            imageState = NORMAL_ICON;
        }
        else if(itemState & ITEM_DISABLED)
        {
            pen = PEN_DISABLED;
            brush = BRUSH_DARK;

            imageIndex = DISABLED_IMAGE;
            imageState = DISABLED_ICON;
        }
        else if(itemState & ITEM_HOVERED)
        {
            pen = PEN_HOVERED;
            brush = BRUSH_HOVERED;

            imageIndex = HOVERED_IMAGE;
            imageState = HOVERED_ICON;
        }
        else
        {
            pen = PEN_NORMAL;
            brush = BRUSH_LIGHT;

            imageIndex = NORMAL_IMAGE;
            imageState = NORMAL_ICON;
        }

        if(itemState & ITEM_FOCUSED)
            pen = PEN_SELECTED;

        DrawRectangle(dc, rcItem, pen, brush);

        if(m_images[imageIndex] < 0)
            imageIndex = NORMAL_IMAGE;

        if(m_images[imageIndex] >= 0)
        {
            int iconCX = ::GetSystemMetrics(SM_CXSMICON);
            int iconCY = ::GetSystemMetrics(SM_CYSMICON);

            CPoint pt = rcItem.CenterPoint() - CPoint(iconCX / 2, iconCY / 2);
            m_iconManager.DrawIcon(dc, pt, m_images[imageIndex], OVERLAY_NONE, imageState);
        }
    }

    // Implementation

    CInformerBitmapButtonImpl()
    {
        SetImages(-1, -1, -1, -1);
    }

protected:
    int m_images[IMAGE_COUNT];
};

class CInformerBitmapButton :
    public CInformerBitmapButtonImpl<CInformerBitmapButton>
{
public:
    DECLARE_WND_SUPERCLASS(L"InformerBitmapButton", GetWndClassName())
};
