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
    #error InformerListCtrl.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/controls/InformerControl.h>
#include <informer/utility/ScrollInfo.h>


template
<
    class T,
    class TBase = CWindow,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CInformerListCtrlImpl :
    public CInformerControlImpl<T, TBase, TWinTraits>
{
public:
    enum
    {
        ITEM_NORMAL     = 0,
        ITEM_HOVERED    = 1,
        ITEM_SELECTED   = 2
    };

public:
    DECLARE_WND_SUPERCLASS(NULL, GetWndClassName())

    // Operations

    void ResetContent()
    {
        ATLASSERT(::IsWindow(m_hWnd));

        m_itemHeights.clear();

        m_selectedItemID = LB_ERR;
        m_hoveredItemID = LB_ERR;

        m_topIndex = 0;
        m_itemCount = 0;

        UpdateControl(false, true);

        Invalidate();
        UpdateWindow();
    }

    void UpdateItem(UINT itemID)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < m_itemCount);

        MeasureItemHeight(itemID);
        InvalidateItem(itemID);
    }

    void InvalidateItem(UINT itemID)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        CRect rcItem;
        GetItemRect(itemID, rcItem);

        InvalidateRect(rcItem);
    }

    void BeginUpdate()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        SetRedraw(FALSE);
    }

    void EndUpdate()
    {
        ATLASSERT(::IsWindow(m_hWnd));
        SetRedraw(TRUE);
        Invalidate();
        UpdateWindow();
    }

    int GetCurSel() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return m_selectedItemID;
    }

    void SetCurSel(int itemID)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        if(itemID < 0)
            itemID = LB_ERR;

        if(itemID >= m_itemCount)
            itemID = LB_ERR;

        if(itemID != m_selectedItemID)
        {
            if(LB_ERR != m_selectedItemID)
                InvalidateItem(m_selectedItemID);

            m_selectedItemID = itemID;

            if(LB_ERR != m_selectedItemID)
                ScrollIntoView(m_selectedItemID);
            else
                SendNotification(LBN_SELCANCEL);
        }
    }

    UINT GetTopIndex() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return m_topIndex;
    }

    void SetTopIndex(int topIndex)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        m_topIndex = topIndex;
        UpdateControl(false, true);

        Invalidate();
        UpdateWindow();
    }

    void SetCount(UINT itemCount)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        m_itemCount = itemCount;
        m_itemHeights.resize(m_itemCount);

        if(m_hoveredItemID >= m_itemCount)
            m_hoveredItemID = LB_ERR;

        if(m_selectedItemID >= m_itemCount)
            m_selectedItemID = LB_ERR;

        UpdateControl(true, true);

        Invalidate();
        UpdateWindow();
    }

    UINT GetCount() const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        return m_itemCount;
    }

    int ItemFromPoint(CPoint pt) const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        CRect rc;
        GetClientRect(rc);

        if(rc.PtInRect(pt))
        {
            int itemsHeight = 0;
            int itemID = m_topIndex;

            while(itemID < m_itemCount)
            {
                itemsHeight += m_itemHeights[itemID];
                if(itemsHeight > pt.y) break;
                ++itemID;
            }

            if(itemID < m_itemCount)
                return itemID;
        }

        return LB_ERR;
    }

    void GetItemRect(UINT itemID, CRect& rcItem) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        CSize size;
        GetClientSize(size);

        int itemOffest = GetItemOffset(itemID);

        rcItem.SetRect(0, itemOffest, size.cx,
            itemOffest + m_itemHeights[itemID]);
    }

    void ScrollIntoView(UINT itemID)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        CSize size;
        GetClientSize(size);

        int tempID = itemID;

        if(tempID > m_topIndex)
        {
            int itemsHeight = m_itemHeights[tempID];

            while(tempID > m_topIndex)
            {
                itemsHeight += m_itemHeights[tempID - 1];
                if(itemsHeight > size.cy) break;
                --tempID;
            }
        }

        SetTopIndex(tempID);
    }

    int GetItemState(UINT itemID) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        int tempID = itemID;
        int itemState = ITEM_NORMAL;

        if(tempID == m_hoveredItemID)
            itemState |= ITEM_HOVERED;

        if(tempID == m_selectedItemID)
            itemState |= ITEM_SELECTED;

        return itemState;
    }

    void GetIdealSize(CSize& size) const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        if(!m_itemCount)
        {
            GetClientSize(size);
        }
        else
        {
            LONG borderCX = ::GetSystemMetrics(SM_CXEDGE);
            LONG borderCY = ::GetSystemMetrics(SM_CYEDGE);

            size.cx = m_maxItemWidth + borderCX * 2;
            size.cy = GetItemsHeight() + borderCY * 2;
        }
    }

    void GetClientSize(CSize& size) const
    {
        CRect rc;
        GetClientRect(rc);
        size = rc.Size();
    }

    // Message map and event handlers

    BEGIN_MSG_MAP(CInformerListCtrlImpl)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_LBUTTONDOWN(OnButtonDown)
        MSG_WM_RBUTTONDOWN(OnButtonDown)
        MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
        MSG_WM_KEYDOWN(OnKeyDown)
        MSG_WM_VSCROLL(OnVScroll)
        MSG_WM_MOUSEWHEEL(OnMouseWheel)
        MSG_WM_GETDLGCODE(OnGetDlgCode)
        MSG_WM_SIZE(OnSize)
        MSG_WM_SETFOCUS(OnSetFocus)
        MSG_WM_KILLFOCUS(OnKillFocus)
        CHAIN_MSG_MAP(CInformerControlImpl)
    END_MSG_MAP()

    void OnMouseMove(UINT /*nFlags*/, CPoint pt)
    {
        HWND hWndParent = GetParent();
        
        if(::GetForegroundWindow() == hWndParent)
        {
            if(::GetFocus() != m_hWnd)
                ::SetFocus(m_hWnd);
        }

        UpdateHoveredItem(pt);
        SetMsgHandled(FALSE);
    }

    void OnButtonDown(UINT /*nFlags*/, CPoint pt)
    {
        SetFocus();
        SetCurSel(ItemFromPoint(pt));
    }

    void OnLButtonDblClk(UINT /*nFlags*/, CPoint /*pt*/)
    {
        SetFocus();
        SendNotification(LBN_DBLCLK);
    }

    void OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
    {
        if(m_itemCount > 0)
        {
            int pageSize = GetItemsPerPage();
            int selectedItemID = m_selectedItemID;

            switch(nChar)
            {
                case VK_UP:
                case VK_LEFT:
                    if(LB_ERR == selectedItemID)
                        selectedItemID = m_itemCount - 1;
                    else
                        --selectedItemID;
                    break;

                case VK_DOWN:
                case VK_RIGHT:
                    if(LB_ERR == selectedItemID)
                        selectedItemID = 0;
                    else
                        ++selectedItemID;
                    break;

                case VK_PRIOR:
                    if(LB_ERR == selectedItemID)
                        selectedItemID = m_itemCount - 1;
                    selectedItemID -= pageSize;
                    break;

                case VK_NEXT:
                    if(LB_ERR == selectedItemID)
                        selectedItemID = 0;
                    selectedItemID += pageSize;
                    break;

                case VK_HOME:
                    selectedItemID = 0;
                    break;

                case VK_END:
                    selectedItemID = m_itemCount - 1;
                    break;

                case VK_RETURN:
                    SendNotification(LBN_DBLCLK);
                    break;

                default:
                    SetMsgHandled(FALSE);
            }

            if(selectedItemID != m_selectedItemID)
            {
                if(selectedItemID < 0)
                    selectedItemID = 0;

                if(selectedItemID >= m_itemCount)
                    selectedItemID = m_itemCount - 1;

                SetCurSel(selectedItemID);
                SendNotification(LBN_SELCHANGE);
            }
        }
    }

    void OnVScroll(UINT nSBCode, UINT nPos, HWND /*hWndScrollBar*/)
    {
        CScrollInfo scrollInfo(SIF_TRACKPOS);

        switch(nSBCode)
        {
            case SB_TOP:
                SetTopIndex(0);
                break;

            case SB_BOTTOM:
                SetTopIndex(m_itemCount);
                break;

            case SB_LINEUP:
                SetTopIndex(m_topIndex - 1);
                break;

            case SB_LINEDOWN:
                SetTopIndex(m_topIndex + 1);
                break;

            case SB_PAGEUP:
                SetTopIndex(m_topIndex - GetItemsPerPage());
                break;

            case SB_PAGEDOWN:
                SetTopIndex(m_topIndex + GetItemsPerPage());
                break;

            case SB_THUMBPOSITION:
                SetTopIndex(nPos);
                break;

            case SB_THUMBTRACK:
                GetScrollInfo(SB_VERT, scrollInfo);
                SetTopIndex(scrollInfo.nTrackPos);
                break;
        }
    }

    BOOL OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
    {
        SetTopIndex(m_topIndex + (-zDelta / WHEEL_DELTA) * 3);
        return TRUE;
    }

    UINT OnGetDlgCode(LPMSG /*lpMsg*/)
    {
        return DLGC_WANTALLKEYS;
    }

    void OnSize(UINT /*nType*/, CSize /*size*/)
    {
        UpdateControl(true, false);
    }

    void OnSetFocus(HWND /*hWndOld*/)
    {
        UpdateControl(false, true);
        SendNotification(LBN_SETFOCUS);
    }

    void OnKillFocus(HWND /*hWndFocus*/)
    {
        SendNotification(LBN_KILLFOCUS);
    }

    // Implementation

    CInformerListCtrlImpl() :
        m_itemCount(0),
        m_topIndex(0),
        m_hoveredItemID(LB_ERR),
        m_selectedItemID(LB_ERR),
        m_maxItemWidth(0)
    {
        EnableDoubleBuffering();
    }

    void Init()
    {
        CInformerControlImpl<T, TBase, TWinTraits>::Init();
        ModifyStyle(0, LBS_NOINTEGRALHEIGHT);
    }

    void UpdateControl(bool measureItems, bool updateHoveredItem)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        if(measureItems)
            MeasureAllItemSizes();

        if(UpdateTopIndex())
            updateHoveredItem = true;

        UpdateVertScrollBar();

        if(updateHoveredItem)
            UpdateHoveredItem();
    }

    void MeasureAllItemSizes()
    {
        ATLASSERT(::IsWindow(m_hWnd));

        if(m_itemCount > 0)
        {
            CClientDC dcClient(m_hWnd);
            CDCHandle dc(dcClient);

            int scrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
            bool hasScroll = (GetStyle() & WS_VSCROLL) == WS_VSCROLL;

            T* pT = static_cast<T*>(this);

            CSize size;
            GetClientSize(size);

            int itemsWidth = size.cx;

            for(;;)
            {
                m_maxItemWidth = 0;

                int itemsHeight = 0;
                bool restart = false;

                for(int itemID = 0; itemID < m_itemCount; ++itemID)
                {
                    CSize itemSize(itemsWidth, 0);
                    pT->MeasureItem(itemID, dc, itemSize);

                    m_itemHeights[itemID] = itemSize.cy;

                    if(itemSize.cx > m_maxItemWidth)
                        m_maxItemWidth = itemSize.cx;

                    if(!hasScroll)
                    {
                        itemsHeight += itemSize.cy;

                        if(itemsHeight > size.cy)
                        {
                            itemsWidth -= scrollWidth;
                            hasScroll = true;
                            restart = true;
                        }
                    }
                }

                if(!restart)
                    break;
            }
        }
    }

    void SendNotification(int notifyCode)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        ::SendMessage(GetParent(), WM_COMMAND,
            MAKEWPARAM(GetWindowLongPtr(GWLP_ID),
            notifyCode), reinterpret_cast<LPARAM>(m_hWnd));
    }

    void UpdateHoveredItem()
    {
        ATLASSERT(::IsWindow(m_hWnd));

        CPoint pt;

        ::GetCursorPos(&pt);
        ScreenToClient(&pt);

        UpdateHoveredItem(pt);
    }

    void UpdateHoveredItem(CPoint pt)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        int itemID = ItemFromPoint(pt);

        if(itemID != m_hoveredItemID)
        {
            if(LB_ERR != m_hoveredItemID)
                InvalidateItem(m_hoveredItemID);

            m_hoveredItemID = itemID;

            if(LB_ERR != m_hoveredItemID)
                InvalidateItem(m_hoveredItemID);
        }
    }

    bool UpdateTopIndex()
    {
        int maxTopIndex = GetMaxTopIndex();

        if(m_topIndex > maxTopIndex)
        {
            m_topIndex = maxTopIndex;
            return true;
        }

        if(m_topIndex < 0)
        {
            m_topIndex = 0;
            return true;
        }

        return false;
    }

    void UpdateVertScrollBar()
    {
        ATLASSERT(::IsWindow(m_hWnd));

        int nPos = m_topIndex;
        int nMax = m_itemCount - 1;
        int nPage = GetItemsPerPage();

        CScrollInfo scrollInfo(SIF_ALL, 0, nMax, nPage, nPos);
        SetScrollInfo(SB_VERT, scrollInfo, TRUE);
    }

    int GetItemOffset(UINT itemID) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        int itemOffset = 0;
        int tempID = itemID;

        if(tempID < m_topIndex)
        {
            while(tempID < m_topIndex)
                itemOffset -= m_itemHeights[tempID++];
        }
        else
        {
            while(--tempID >= m_topIndex)
                itemOffset += m_itemHeights[tempID];
        }

        return itemOffset;
    }

    int GetItemsPerPage() const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        CSize size;
        GetClientSize(size);

        int itemsHeight = 0;
        int itemID = m_topIndex;

        while(itemID < m_itemCount)
        {
            itemsHeight += m_itemHeights[itemID];
            if(itemsHeight > size.cy) break;
            ++itemID;
        }

        if(itemID > m_topIndex)
            return (itemID - m_topIndex);

        return 1;
    }

    int GetMaxTopIndex() const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        CSize size;
        GetClientSize(size);

        int clientHeight = size.cy;
        int maxTopIndex = m_itemCount - 1;

        while(maxTopIndex >= 0)
        {
            clientHeight -= m_itemHeights[maxTopIndex];
            if(clientHeight < 0) break;
            --maxTopIndex;
        }

        if(maxTopIndex < m_itemCount - 1)
            ++maxTopIndex;

        if(maxTopIndex < 0)
            maxTopIndex = 0;

        return maxTopIndex;
    }

    int GetItemsHeight() const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        int itemsHeight = 0;
        int itemCount = m_itemCount;

        while(--itemCount >= 0)
            itemsHeight += m_itemHeights[itemCount];

        return itemsHeight;
    }

    // Overrideables

    void DrawControl(CDCHandle dc, CRect& rc)
    {
        CRect rcTemp;
        CRect rcClient;

        GetClientRect(rcClient);

        T* pT = static_cast<T*>(this);
        pT->DrawBackground(dc, rc);

        for(int itemID = m_topIndex; itemID < m_itemCount; ++itemID)
        {
            CRect rcItem;
            GetItemRect(itemID, rcItem);

            if(rcTemp.IntersectRect(rc, rcItem))
            {
                int itemState = GetItemState(itemID);
                pT->DrawItem(itemID, dc, rcItem, itemState);
            }
            else if(!rcTemp.IntersectRect(rcClient, rcItem))
            {
                break;
            }
        }
    }

    void HandleMouseLeave()
    {
        CPoint ptInvalid(-1, -1);
        UpdateHoveredItem(ptInvalid);
    }

    void MeasureItem(UINT /*itemID*/, CDCHandle /*dc*/, CSize& /*itemSize*/)
    {
        // must be implemented
        ATLASSERT(FALSE);
    }

    void DrawItem(UINT /*itemID*/, CDCHandle /*dc*/, CRect& /*rcItem*/, int /*itemState*/)
    {
        // must be implemented
        ATLASSERT(FALSE);
    }

    void DrawBackground(CDCHandle /*dc*/, CRect const& /*rcClient*/)
    {
        // must be implemented
        ATLASSERT(FALSE);
    }

private:
    int m_itemCount;
    int m_topIndex;

    int m_hoveredItemID;
    int m_selectedItemID;

    int m_maxItemWidth;
    std::vector<int> m_itemHeights;
};

class CInformerListCtrl :
    public CInformerListCtrlImpl<CInformerListCtrl>
{
public:
    DECLARE_WND_SUPERCLASS(L"InformerListCtrl", GetWndClassName())
};
