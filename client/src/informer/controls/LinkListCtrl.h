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
    #error LinkListCtrl.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/resources/resource.h>
#include <informer/controls/InformerListCtrl.h>
#include <informer/utility/ShellExecuteHelper.h>
#include <core/link_list.hpp>


template
<
    class T,
    class TBase = CWindow,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CLinkListCtrlImpl :
    public CInformerListCtrlImpl<T, TBase, TWinTraits>
{
public:
    enum
    {
        NAME_OFFSET         = 12,
        DESCR_OFFSET        = 24,

        LIST_HEIGHT_SPACING = 4,
        LIST_WIDTH_SPACING  = NAME_OFFSET + DESCR_OFFSET
    };

public:
    static UINT const drawTextFormat = DT_NOCLIP |
        DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    void Assign(missio::link_list const& links)
    {
        int curSel = GetCurSel();

        m_links.assign(links);

        BeginUpdate();
        SetCount(m_links.size());
        SetCurSel(curSel);
        EndUpdate();
    }

    void Clear()
    {
        m_links.clear();
        ResetContent();
    }

    // Message map and event handlers

    BEGIN_MSG_MAP(CLinkListCtrlImpl)
        MSG_WM_COMMAND(OnCommand)
        MSG_OCM_COMMAND(OnCommand)
        CHAIN_MSG_MAP(CInformerListCtrlImpl)
    END_MSG_MAP()

    void OnCommand(UINT uNotifyCode, int /*nID*/, HWND /*hWnd*/)
    {
        if(LBN_DBLCLK == uNotifyCode)
            NavigateToLinkUrl(GetCurSel());
    }

    // Implementation

    CLinkListCtrlImpl() :
        m_itemNameTextWidth(-1)
    {
    }

    void NavigateToLinkUrl(UINT itemID)
    {
        if(LB_ERR != itemID)
        {
            missio::link const& link = m_links[itemID];
            CShellExecuteHelper(link.uri()).Start();
        }
    }

    // CInformerListCtrlImpl

    void MeasureItem(UINT itemID, CDCHandle dc, CSize& itemSize)
    {
        CRect rcText;

        missio::link const& link = m_links[itemID];

        CFontHandle newFont = m_drawManager.Font(FONT_MAIN);
        CFontHandle oldFont = dc.SelectFont(newFont);

        dc.DrawText(
            link.name().c_str(),
            link.name().size(),
            rcText,
            drawTextFormat | DT_CALCRECT);

        m_itemNameTextWidth = max(m_itemNameTextWidth, rcText.Width());

        dc.DrawText(
            link.descr().c_str(),
            link.descr().size(),
            rcText,
            drawTextFormat | DT_CALCRECT);

        TEXTMETRIC tm;
        dc.GetTextMetrics(&tm);

        dc.SelectFont(oldFont);

        LONG iconCX = ::GetSystemMetrics(SM_CXSMICON);
        LONG iconCY = ::GetSystemMetrics(SM_CYSMICON);

        itemSize.cy = max(iconCY, tm.tmHeight) + LIST_HEIGHT_SPACING;
        itemSize.cx = iconCX + m_itemNameTextWidth + rcText.Width() + LIST_WIDTH_SPACING;
    }

    void DrawItem(UINT itemID, CDCHandle dc, CRect& rcItem, int itemState)
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
        else if(itemState & ITEM_HOVERED)
        {
            pen = PEN_INVALID;
            brush = BRUSH_HOVERED;
            textColor = COLOR_TEXT_NORMAL;
        }
        else
        {
            pen = PEN_INVALID;
            brush = BRUSH_LIGHT;
            textColor = COLOR_TEXT_NORMAL;
        }

        DrawRectangle(dc, rcItem, pen, brush);

        missio::link const& link = m_links[itemID];

        int iconCX = ::GetSystemMetrics(SM_CXSMICON);
        int iconCY = ::GetSystemMetrics(SM_CYSMICON);

        CPoint pt(2, rcItem.top + (rcItem.Height() - iconCY) / 2);
        m_iconManager.DrawIcon(dc, pt, link.uri(), OVERLAY_NONE, NORMAL_ICON);

        CRect rcName(iconCX + rcItem.left + NAME_OFFSET,
            rcItem.top, rcItem.right, rcItem.bottom);

        CRect rcDescr(iconCX + m_itemNameTextWidth + DESCR_OFFSET,
            rcItem.top, rcItem.right, rcItem.bottom);

        dc.SetBkMode(TRANSPARENT);

        dc.SelectFont(m_drawManager.Font(FONT_MAIN));
        dc.SetTextColor(m_drawManager.Color(textColor));

        dc.DrawText(
            link.name().c_str(),
            link.name().size(),
            rcName,
            drawTextFormat);

        dc.DrawText(
            link.descr().c_str(),
            link.descr().size(),
            rcDescr,
            drawTextFormat);
    }

    void DrawBackground(CDCHandle dc, CRect const& rcClient)
    {
        DrawFillRect(dc, rcClient, BRUSH_LIGHT);
    }

private:
    int m_itemNameTextWidth;
    missio::link_list m_links;
};

class CLinkListCtrl :
    public CLinkListCtrlImpl<CLinkListCtrl>
{
public:
    DECLARE_WND_SUPERCLASS(L"LinkListCtrl", GetWndClassName())
};
