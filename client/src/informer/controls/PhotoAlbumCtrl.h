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
    #error PhotoAlbumCtrl.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/controls/InformerControl.h>
#include <informer/controls/InformerMenu.h>
#include <informer/controls/InformerBitmap.h>
#include <informer/utility/InformerImageList.h>
#include <informer/utility/ShellExecuteHelper.h>
#include <informer/utility/ScrollInfo.h>
#include <core/photo_list.hpp>


template
<
    class T,
    class TBase = CWindow,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CPhotoAlbumCtrlImpl :
    public CInformerControlImpl<T, TBase, TWinTraits>
{
public:
    enum
    {
        ITEM_NORMAL     = 0,
        ITEM_HOVERED    = 1,
        ITEM_SELECTED   = 2
    };

    enum
    {
        THUMB_WIDTH     = 100,
        THUMB_HEIGHT    = 75,
        THUMB_TEXT      = 30,
        THUMB_INDENT    = 10,

        PHOTO_SPACE     = 5,

        PHOTO_WIDTH     = THUMB_WIDTH + THUMB_INDENT * 2,
        PHOTO_HEIGHT    = THUMB_HEIGHT + THUMB_TEXT + THUMB_INDENT * 2,

        TEXT_OFFSET     = THUMB_HEIGHT + THUMB_INDENT,
        SCROLL_LINE     = (PHOTO_HEIGHT + PHOTO_SPACE) / 2,

        IDEAL_WIDTH     = (PHOTO_WIDTH + PHOTO_SPACE) * 5 + PHOTO_SPACE * 2 - 1,
        IDEAL_HEIGHT    = (PHOTO_HEIGHT + PHOTO_SPACE) * 4 + PHOTO_SPACE * 2 - 1
    };

public:
    // Controls
    CInformerMenu m_photoMenu;

public:
    static UINT const drawTextFormat = DT_NOCLIP | DT_CENTER | DT_WORDBREAK | DT_NOPREFIX;

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    void Assign(chat::photo_list const& photos)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        for(std::size_t i = 0; i < m_photos.size(); ++i)
        {
            if(!photos.contains(m_photos[i].id()))
                m_images.Remove(m_photos[i].id());
        }

        //m_photos.assign(photos);
        m_itemCount = m_photos.size();
        m_itemRects.resize(m_itemCount);

        if(m_hoveredItemID >= m_itemCount)
            m_hoveredItemID = LB_ERR;

        if(m_selectedItemID >= m_itemCount)
            m_selectedItemID = LB_ERR;

        ReloadPhotoThumbnailImages();
        UpdateControl(true, true, true);
    }

    void GetIdealSize(CSize& size) const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        size.cx = IDEAL_WIDTH;
        size.cy = IDEAL_HEIGHT;

        if(GetStyle() & WS_VSCROLL)
            size.cx += ::GetSystemMetrics(SM_CXVSCROLL);

        if(GetStyle() & WS_HSCROLL)
            size.cy += ::GetSystemMetrics(SM_CYHSCROLL);
    }

    void UpdatePhotoThumbnail(std::wstring const& photoID)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        if(m_photos.contains(photoID))
            UpdatePhotoThumbnail(m_photos[photoID]);
    }

    void UpdatePhotoThumbnail(chat::photo const& photo)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        std::wstring photoID = photo.id();
        std::wstring thumb = photo.thumb();

        if(!m_images.Contains(photoID))
        {
            CInformerBitmap imgThumb;

            boost::filesystem::path filename = "data/photos";
            filename /= boost::filesystem::path(thumb);

            if(imgThumb.LoadFromFile(filename.c_str()))
            {
                m_images.Add(photoID, imgThumb);
                InvalidateItem(photoID);
            }
        }
    }

    void InvalidateItem(std::wstring const& photoID)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        std::size_t index = m_photos.index_of(photoID);

        if(chat::photo_list::invalid_index != index)
            InvalidateItem(static_cast<UINT>(index));
    }

    void InvalidateItem(UINT itemID)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        CRect rcItem;
        GetItemRect(itemID, rcItem);

        InvalidateRect(rcItem);
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
            {
                if(IsItemVisible(m_selectedItemID))
                    InvalidateItem(m_selectedItemID);
                else
                    ScrollIntoView(m_selectedItemID);
            }
        }
    }

    int ItemFromPoint(CPoint pt) const
    {
        ATLASSERT(::IsWindow(m_hWnd));

        CRect rcClient;
        GetClientRect(rcClient);

        for(int itemID = 0; itemID < m_itemCount; ++itemID)
        {
            CRect rcItem;
            GetItemRect(itemID, rcItem);

            if(rcClient.PtInRect(pt))
            {
                if(rcItem.PtInRect(pt))
                    return itemID;
            }
        }

        return LB_ERR;
    }

    void GetItemRect(UINT itemID, CRect& rcItem) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        rcItem.CopyRect(m_itemRects[itemID]);
        rcItem.OffsetRect(0, -m_scrollOffset);
    }

    int GetItemState(UINT itemID) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        int tempID = itemID;
        int itemState = ITEM_NORMAL;

        if(LB_ERR != m_hoveredItemID)
        {
            if(tempID == m_hoveredItemID)
                itemState |= ITEM_HOVERED;
        }

        if(LB_ERR != m_selectedItemID)
        {
            if(tempID == m_selectedItemID)
                itemState |= ITEM_SELECTED;
        }

        return itemState;
    }

    bool SetScrollOffset(int scrollOffset)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        CSize size;
        GetClientSize(size);

        int maxScrollOffset = m_itemsHeight - size.cy;

        if(scrollOffset > maxScrollOffset)
            scrollOffset = maxScrollOffset;

        if(scrollOffset < 0)
            scrollOffset = 0;

        if(scrollOffset != m_scrollOffset)
        {
            m_scrollOffset = scrollOffset;
            UpdateControl(false, true, true);

            return true;
        }

        return false;
    }

    void GetClientSize(CSize& size) const
    {
        CRect rc;
        GetClientRect(rc);
        size = rc.Size();
    }

    // Message map and handlers

    BEGIN_MSG_MAP(CPhotoAlbumCtrlImpl)
        MSG_WM_CONTEXTMENU(OnContextMenu)
        MSG_WM_COMMAND(OnCommand)
        MSG_OCM_COMMAND(OnCommand)
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
        CHAIN_MSG_MAP_MEMBER(m_photoMenu)
        CHAIN_MSG_MAP(CInformerControlImpl)
    END_MSG_MAP()

    void OnContextMenu(HWND /*hWnd*/, CPoint pt)
    {
        if(pt.x < 0 || pt.y < 0)
        {
            int itemID = GetCurSel();

            if(LB_ERR != itemID)
            {
                CRect rc;
                GetItemRect(itemID, rc);
                ClientToScreen(rc);

                pt.x = rc.left;
                pt.y = rc.bottom + 2;
            }
        }

        ShowPopupMenu(pt);
        SetMsgHandled(FALSE);
    }

    void OnCommand(UINT uNotifyCode, int nID, HWND /*hWnd*/)
    {
        if(0 == uNotifyCode)
        {
            switch(nID)
            {
                case IDC_OPENPHOTO:
                    NavigateToCurrentPhotoUrl();
                    break;

                case IDC_USERINFO:
                    ShowCurrentPhotoUserInfo();
                    break;

                default:
                    SetMsgHandled(FALSE);
            }
        }
        else
        {
            SetMsgHandled(FALSE);
        }
    }

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
        //TODO: SetFocus(); // remove if OnButtonDown is being called just before this
        NavigateToCurrentPhotoUrl();
    }

    void OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
    {
        if(m_itemCount > 0)
        {
            int selectedItemID = m_selectedItemID;

            switch(nChar)
            {
                case VK_UP:
                    if(LB_ERR == selectedItemID)
                        selectedItemID = m_itemCount - 1;
                    else if(selectedItemID >= m_colCount)
                        selectedItemID -= m_colCount;
                    break;

                case VK_LEFT:
                    if(LB_ERR == selectedItemID)
                        selectedItemID = m_itemCount - 1;
                    else
                        --selectedItemID;
                    break;

                case VK_DOWN:
                    if(LB_ERR == selectedItemID)
                        selectedItemID = 0;
                    else if(selectedItemID < m_itemCount - m_colCount)
                        selectedItemID += m_colCount;
                    break;

                case VK_RIGHT:
                    if(LB_ERR == selectedItemID)
                        selectedItemID = 0;
                    else
                        ++selectedItemID;
                    break;

                case VK_RETURN:
                    NavigateToCurrentPhotoUrl();
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
            }
        }
    }

    void OnVScroll(UINT nSBCode, UINT nPos, HWND /*hWndScrollBar*/)
    {
        CSize size;
        GetClientSize(size);

        CScrollInfo scrollInfo(SIF_TRACKPOS);

        switch(nSBCode)
        {
            case SB_TOP:
                SetScrollOffset(0);
                break;

            case SB_BOTTOM:
                SetScrollOffset(m_itemsHeight);
                break;

            case SB_LINEUP:
                SetScrollOffset(m_scrollOffset - SCROLL_LINE);
                break;

            case SB_LINEDOWN:
                SetScrollOffset(m_scrollOffset + SCROLL_LINE);
                break;

            case SB_PAGEUP:
                SetScrollOffset(m_scrollOffset - size.cy);
                break;

            case SB_PAGEDOWN:
                SetScrollOffset(m_scrollOffset + size.cy);
                break;

            case SB_THUMBPOSITION:
                SetScrollOffset(nPos);
                break;

            case SB_THUMBTRACK:
                GetScrollInfo(SB_VERT, scrollInfo);
                SetScrollOffset(scrollInfo.nTrackPos);
                break;
        }
    }

    BOOL OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
    {
        SetScrollOffset(m_scrollOffset + (-zDelta / WHEEL_DELTA) * SCROLL_LINE);
        return TRUE;
    }

    UINT OnGetDlgCode(LPMSG /*lpMsg*/)
    {
        return DLGC_WANTALLKEYS;
    }

    void OnSize(UINT /*nType*/, CSize /*size*/)
    {
        UpdateControl(true, false, true);
    }

    void OnSetFocus(HWND /*hWndOld*/)
    {
        UpdateControl(false, true, false);
    }

    void OnKillFocus(HWND /*hWndFocus*/)
    {
        // If the control is subclassed or superclassed, this message can cause
        // painting of unwanted item frame. We just ignore this message.
    }

    // Implementation

    CPhotoAlbumCtrlImpl() :
        m_colCount(0),
        m_rowCount(0),
        m_itemCount(0),
        m_itemsHeight(0),
        m_scrollOffset(0),
        m_hoveredItemID(LB_ERR),
        m_selectedItemID(LB_ERR)
    {
        EnableDoubleBuffering();
    }

    void Init()
    {
        CInformerControlImpl<T, TBase, TWinTraits>::Init();
        ModifyStyle(0, LBS_NOINTEGRALHEIGHT);

        // Create user popup menu

        ATLVERIFY(m_photoMenu.LoadMenu(IDR_PHOTOMENU));
        CInformerMenuHandle photoMenu = m_photoMenu.GetSubMenu(0);

        photoMenu.SetMenuItemImage(IDC_OPENPHOTO, FALSE, ICON_PHOTO, OVERLAY_NONE);
        photoMenu.SetMenuItemImage(IDC_USERINFO, FALSE, ICON_INFO, OVERLAY_NONE);
        photoMenu.SetMenuDefaultItem(IDC_OPENPHOTO);

        // Create image list

        m_images.Create(THUMB_WIDTH, THUMB_HEIGHT);
        ATLASSERT(!m_images.IsNull());
    }

    void ShowPopupMenu(CPoint pt)
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            chat::photo const& photo = m_photos[itemID];
            std::wstring const& nickname = photo.nickname();

            if(!nickname.empty())
                SetPhotoMenuItemEnabled(IDC_USERINFO, TRUE);
            else
                SetPhotoMenuItemEnabled(IDC_USERINFO, FALSE);

            m_photoMenu.GetSubMenu(0).TrackPopupMenuEx(
                TPM_TOPALIGN, pt.x, pt.y, m_hWnd);
        }
    }

    void SetPhotoMenuItemEnabled(UINT itemID, BOOL enabled)
    {
        UINT enable = enabled ? MF_ENABLED : MF_DISABLED;
        m_photoMenu.GetSubMenu(0).EnableMenuItem(itemID, enable);
    }

    void SetPhotoMenuItemString(UINT itemID, UINT stringID)
    {
        CString strTemp(reinterpret_cast<LPCWSTR>(stringID));
        CInformerMenuHandle menu = m_photoMenu.GetSubMenu(0);
        menu.SetMenuItemString(itemID, FALSE, strTemp);
    }

    void SetPhotoMenuItemImage(UINT itemID, int icon, int overlay)
    {
        CInformerMenuHandle menu = m_photoMenu.GetSubMenu(0);
        menu.SetMenuItemImage(itemID, FALSE, icon, overlay);
    }

    void NavigateToCurrentPhotoUrl()
    {
        if(LB_ERR != m_selectedItemID)
            NavigateToPhotoUrl(m_photos[m_selectedItemID]);
    }

    void ShowCurrentPhotoUserInfo()
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            chat::photo const& photo = m_photos[itemID];
            std::wstring const& nickname = photo.nickname();

            if(!nickname.empty())
                m_dialogManager.ShowUserInfo(nickname.c_str());
        }
    }

    void NavigateToPhotoUrl(chat::photo const& photo)
    {
        //CShellExecuteHelper(photo.get_photo_url()).Start();
    }

    void UpdateControl(bool measureItems,
                       bool updateHoveredItem,
                       bool invalidateClient)
    {
        ATLASSERT(::IsWindow(m_hWnd));

        if(measureItems)
            MeasureAllItemsRects();

        if(SetScrollOffset(m_scrollOffset))
            updateHoveredItem = true;

        if(updateHoveredItem)
            UpdateHoveredItem();

        UpdateVertScrollBar();

        if(invalidateClient)
        {
            Invalidate();
            UpdateWindow();
        }
    }

    void MeasureAllItemsRects()
    {
        ATLASSERT(::IsWindow(m_hWnd));

        CSize size;
        GetClientSize(size);

        m_colCount = 0;
        m_rowCount = 0;

        m_itemsHeight = 0;

        if(m_itemCount > 0)
        {
            m_colCount = size.cx - PHOTO_SPACE;
            m_colCount /= PHOTO_WIDTH + PHOTO_SPACE;

            if(m_colCount > 0)
            {
                m_rowCount = m_itemCount / m_colCount;

                if(m_itemCount % m_colCount)
                    ++m_rowCount;
            }

            LONG itemsHeight = m_rowCount * (PHOTO_HEIGHT + PHOTO_SPACE);
            m_itemsHeight = max(itemsHeight + PHOTO_SPACE, size.cy);

            int itemID = 0;

            for(int row = 0; row < m_rowCount; ++row)
            {
                for(int col = 0; col < m_colCount; ++col)
                {
                    int x = col * (PHOTO_WIDTH + PHOTO_SPACE) + PHOTO_SPACE;
                    int y = row * (PHOTO_HEIGHT + PHOTO_SPACE) + PHOTO_SPACE;

                    m_itemRects[itemID].SetRect(x, y, x + PHOTO_WIDTH, y + PHOTO_HEIGHT);

                    if(++itemID >= m_itemCount)
                        break;
                }
            }
        }
    }

    void ReloadPhotoThumbnailImages()
    {
        ATLASSERT(::IsWindow(m_hWnd));

        for(int itemID = 0; itemID < m_itemCount; ++itemID)
            UpdatePhotoThumbnail(m_photos[itemID]);
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

    void UpdateVertScrollBar()
    {
        ATLASSERT(::IsWindow(m_hWnd));

        CSize size;
        GetClientSize(size);

        int nPage = size.cy;
        int nPos = m_scrollOffset;
        int nMax = m_itemsHeight - 1;

        UINT fMask = SIF_ALL | SIF_DISABLENOSCROLL;
        CScrollInfo scrollInfo(fMask, 0, nMax, nPage, nPos);
        SetScrollInfo(SB_VERT, scrollInfo, TRUE);
    }

    void ScrollIntoView(UINT itemID)
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        CSize size;
        GetClientSize(size);

        int scrollOffset = m_scrollOffset;

        int itemOffset = m_itemRects[itemID].top - PHOTO_SPACE;
        int itemHeight = m_itemRects[itemID].Height() + PHOTO_SPACE * 2;

        if(scrollOffset > itemOffset)
            scrollOffset = itemOffset;

        if(scrollOffset + size.cy < itemOffset + itemHeight)
            scrollOffset = itemOffset + itemHeight - size.cy;

        SetScrollOffset(scrollOffset);
        UpdateControl(false, true, true);
    }

    bool IsItemVisible(UINT itemID) const
    {
        ATLASSERT(::IsWindow(m_hWnd));
        ATLASSERT(itemID < UINT(m_itemCount));

        CRect rc;
        GetClientRect(rc);
        rc.OffsetRect(0, m_scrollOffset);

        return rc == (rc | m_itemRects[itemID]);
    }

    // Overrideables

    void DrawControl(CDCHandle dc, CRect& rc)
    {
        CRect rcClient;
        GetClientRect(rcClient);

        DrawFillRect(dc, rcClient, BRUSH_DARK);

        for(int itemID = 0; itemID < m_itemCount; ++itemID)
        {
            CRect rcItem;
            GetItemRect(itemID, rcItem);

            if(rcClient.IntersectRect(rc, rcItem))
            {
                int itemState = GetItemState(itemID);
                DrawItem(itemID, dc, rcItem, itemState);
            }
        }
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
            pen = PEN_STATIC;
            brush = BRUSH_HOVERED;
            textColor = COLOR_TEXT_HOVERED;
        }
        else
        {
            pen = PEN_STATIC;
            brush = BRUSH_WHITE;
            textColor = COLOR_TEXT_NORMAL;
        }

        DrawRectangle(dc, rcItem, pen, brush);

        CSize size(THUMB_WIDTH, THUMB_TEXT);
        CRect rcText(rcItem.TopLeft(), size);
        rcText.OffsetRect(0, TEXT_OFFSET);

        std::wstring const& photoID = m_photos[itemID].id();
        std::wstring const& descr = m_photos[itemID].descr();

        dc.SetBkMode(TRANSPARENT);
        dc.SelectFont(m_drawManager.Font(FONT_MAIN));
        dc.SetTextColor(m_drawManager.Color(textColor));

        dc.DrawText(
            descr.c_str(),
            descr.size(),
            rcText,
            drawTextFormat | DT_CALCRECT);

        if(rcText.Width() >= THUMB_WIDTH
            || rcText.Height() >= THUMB_TEXT)
        {
            dc.SelectFont(m_drawManager.Font(FONT_SMALL));

            dc.DrawText(descr.c_str(), descr.size(), rcText, drawTextFormat | DT_CALCRECT);
        }

        rcText.OffsetRect((PHOTO_WIDTH - rcText.Width()) / 2,
            (PHOTO_HEIGHT - TEXT_OFFSET - rcText.Height()) / 2);

        dc.DrawText(descr.c_str(), descr.size(), rcText, drawTextFormat);

        CPoint pt = rcItem.TopLeft();
        pt.Offset(THUMB_INDENT, THUMB_INDENT);

        m_images.Draw(dc, photoID, pt);
    }

    void HandleMouseLeave()
    {
        CPoint ptInvalid(-1, -1);
        UpdateHoveredItem(ptInvalid);
    }

private:
    int m_colCount;
    int m_rowCount;

    int m_itemCount;
    int m_itemsHeight;
    int m_scrollOffset;

    int m_hoveredItemID;
    int m_selectedItemID;

    chat::photo_list m_photos;

    std::vector<CRect> m_itemRects;
    CInformerImageList<std::wstring> m_images;
};

class CPhotoAlbumCtrl :
    public CPhotoAlbumCtrlImpl<CPhotoAlbumCtrl>
{
public:
    DECLARE_WND_SUPERCLASS(L"PhotoAlbumCtrl", GetWndClassName())
};
