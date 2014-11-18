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
    #error InformerMenu.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/utility/InformerHelper.h>
#include <informer/utility/SaveDC.h>


template <bool t_bManaged>
class ATL_NO_VTABLE CInformerMenuT :
    public CMenuT<t_bManaged>,
    public CInformerHelper
{
public:
    struct ITEM_DATA
    {
        UINT fType;
        int nImage;
        int nOverlay;
    };

    enum
    {
        MENU_WIDTH_SPACING      = 16,
        MENU_HEIGHT_SPACING     = 4,
        MENU_TEXT_SPACING       = 8,
        MENU_CAPTION_SPACING    = 12
    };

public:
    static UINT const drawTextFormat = DT_LEFT | DT_VCENTER |
        DT_NOCLIP | DT_SINGLELINE | DT_EXPANDTABS | DT_NOPREFIX;

public:
    // Operations

    CInformerMenuT(HMENU hMenu = NULL) :
        CMenuT(hMenu)
    {
    }

    ~CInformerMenuT()
    {
        if(t_bManaged && m_hMenu != NULL)
            DestroyMenu();
    }

    CInformerMenuT<t_bManaged>& operator=(HMENU hMenu)
    {
        Attach(hMenu);
        return *this;
    }

    void SetMenuItemImage(UINT uItemID, BOOL fByPosition, int nImage, int nOverlay)
    {
        WCHAR szString[MAX_PATH];
        CMenuItemInfo menuItemInfo;
        ITEM_DATA* itemData = NULL;

        menuItemInfo.fMask = MIIM_DATA | MIIM_FTYPE;
        menuItemInfo.dwTypeData = szString;
        menuItemInfo.cch = MAX_PATH;

        ATLVERIFY(GetMenuItemInfo(uItemID, fByPosition, &menuItemInfo));

        if(!(menuItemInfo.fType & MFT_OWNERDRAW))
        {
            itemData = new ITEM_DATA;
            itemData->fType = menuItemInfo.fType;

            menuItemInfo.fType |= MFT_OWNERDRAW;
            menuItemInfo.dwItemData = (ULONG_PTR)itemData;
        }
        else
        {
            itemData = (ITEM_DATA*)menuItemInfo.dwItemData;
        }

        ATLASSERT(itemData != NULL);
        itemData->nImage = nImage;
        itemData->nOverlay = nOverlay;

        ATLVERIFY(SetMenuItemInfo(uItemID, fByPosition, &menuItemInfo));
    }

    void SetMenuItemString(UINT uItemID, BOOL fByPosition, LPCWSTR lpszString)
    {
        CMenuItemInfo menuItemInfo;

        menuItemInfo.fMask = MIIM_STRING;
        menuItemInfo.cch = lstrlen(lpszString);
        menuItemInfo.dwTypeData = (LPWSTR)lpszString;

        ATLVERIFY(SetMenuItemInfo(uItemID, fByPosition, &menuItemInfo));
    }

    // Message map and event handlers

    BEGIN_MSG_MAP_EX(CInformerMenuT<t_bManaged>)
        MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
        MSG_WM_DRAWITEM(OnDrawItem)
        MSG_WM_MEASUREITEM(OnMeasureItem)
    END_MSG_MAP()

    void OnInitMenuPopup(HMENU hMenuPopup, UINT /*nIndex*/, BOOL bSysMenu)
    {
        if(FALSE == bSysMenu)
        {
            if(NULL != hMenuPopup)
                InitMenuPopupHandler(hMenuPopup);
        }
    }

    void OnMeasureItem(int /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
    {
        if(ODT_MENU == lpMeasureItemStruct->CtlType)
        {
            ATLASSERT(lpMeasureItemStruct->itemData != NULL);
            ITEM_DATA* itemData = (ITEM_DATA*)lpMeasureItemStruct->itemData;

            if(itemData->fType & MFT_SEPARATOR)
            {
                lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENU) / 2;
            }
            else
            {
                CString menuString;

                if(GetMenuString(lpMeasureItemStruct->itemID, menuString, MF_BYCOMMAND) > 0)
                {
                    if(!menuString.IsEmpty())
                    {
                        CRect rcText;
                        CClientDC dc(HWND_DESKTOP);

                        CFontHandle newFont = m_drawManager.Font(FONT_MENU_BOLD);
                        CFontHandle oldFont = dc.SelectFont(newFont);

                        dc.DrawText(menuString, -1, rcText, drawTextFormat | DT_CALCRECT);

                        TEXTMETRIC tm;
                        dc.GetTextMetrics(&tm);

                        dc.SelectFont(oldFont);

                        LONG iconCX = ::GetSystemMetrics(SM_CXSMICON);
                        LONG iconCY = ::GetSystemMetrics(SM_CYSMICON);

                        lpMeasureItemStruct->itemWidth = iconCX + rcText.Width() + MENU_WIDTH_SPACING;
                        lpMeasureItemStruct->itemHeight = max(iconCY, tm.tmHeight) + MENU_HEIGHT_SPACING;
                    }
                }
            }
        }
    }

    void OnDrawItem(int /*nIDCtl*/, LPDRAWITEMSTRUCT lpDrawItemStruct)
    {
        if(ODT_MENU == lpDrawItemStruct->CtlType)
        {
            ATLASSERT(lpDrawItemStruct->itemData != NULL);
            ITEM_DATA* itemData = (ITEM_DATA*)lpDrawItemStruct->itemData;

            CDCHandle dc(lpDrawItemStruct->hDC);
            CSaveDC saveDC(lpDrawItemStruct->hDC);
            CRect rcItem(lpDrawItemStruct->rcItem);

            if(itemData->fType & MFT_SEPARATOR)
            {
                dc.FillRect(rcItem, m_drawManager.Brush(BRUSH_LIGHT));
                dc.SelectPen(m_drawManager.Pen(PEN_STATIC));

                int center = rcItem.top + rcItem.Height() / 2;

                dc.MoveTo(rcItem.left + 2, center);
                dc.LineTo(rcItem.right - 2, center);
            }
            else
            {
                bool bDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED) == ODS_DISABLED;
                bool bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED;
                bool bChecked = (lpDrawItemStruct->itemState & ODS_CHECKED) == ODS_CHECKED;
                bool bDefault = (lpDrawItemStruct->itemState & ODS_DEFAULT) == ODS_DEFAULT;

                if(bSelected)
                {
                    if(bDisabled)
                    {
                        dc.SelectPen(m_drawManager.Pen(PEN_STATIC));
                        dc.SelectBrush(m_drawManager.Brush(BRUSH_LIGHT));
                    }
                    else
                    {
                        dc.SelectPen(m_drawManager.Pen(PEN_SELECTED));
                        dc.SelectBrush(m_drawManager.Brush(BRUSH_SELECTED));
                    }

                    dc.Rectangle(rcItem);
                }
                else
                {
                    dc.FillRect(rcItem, m_drawManager.Brush(BRUSH_LIGHT));
                }

                if(bDisabled)
                    dc.SetTextColor(m_drawManager.Color(COLOR_TEXT_DISABLED));
                else if(bSelected)
                    dc.SetTextColor(m_drawManager.Color(COLOR_TEXT_SELECTED));
                else
                    dc.SetTextColor(m_drawManager.Color(COLOR_TEXT_NORMAL));

                int iconCX = ::GetSystemMetrics(SM_CXSMICON);
                int iconCY = ::GetSystemMetrics(SM_CYSMICON);

                int icon = bChecked ? ICON_CHECK : itemData->nImage;
                int iconState = bDisabled ? DISABLED_ICON : NORMAL_ICON;

                if(icon >= 0)
                {
                    CPoint pt(2, rcItem.top + (rcItem.Height() - iconCY) / 2);
                    m_iconManager.DrawIcon(dc, pt, icon, itemData->nOverlay, iconState);
                }

                CString strItem;

                if(GetMenuString(lpDrawItemStruct->itemID, strItem, MF_BYCOMMAND) > 0)
                {
                    if(!strItem.IsEmpty())
                    {
                        int nFont = bDefault ? FONT_MENU_BOLD : FONT_MENU;

                        rcItem.right -= MENU_TEXT_SPACING;
                        rcItem.left += iconCX + MENU_TEXT_SPACING;

                        dc.SetBkMode(TRANSPARENT);
                        dc.SelectFont(m_drawManager.Font(nFont));
                        dc.DrawText(strItem, strItem.GetLength(), rcItem, drawTextFormat);
                    }
                }
            }
        }
    }

    // Implementation

    void InitMenuPopupHandler(HMENU hMenu)
    {
        ATLASSERT(::IsMenu(m_hMenu));
        CMenuHandle popupMenu(hMenu);

        WCHAR szString[MAX_PATH];

        for(int i = 0; i < popupMenu.GetMenuItemCount(); ++i)
        {
            CMenuItemInfo menuItemInfo;

            menuItemInfo.fMask = MIIM_DATA | MIIM_FTYPE;
            menuItemInfo.dwTypeData = szString;
            menuItemInfo.cch = MAX_PATH;

            ATLVERIFY(popupMenu.GetMenuItemInfo(i, TRUE, &menuItemInfo));

            if(!(menuItemInfo.fType & MFT_OWNERDRAW))
            {
                ITEM_DATA* itemData = new ITEM_DATA;

                itemData->fType = menuItemInfo.fType;
                itemData->nOverlay = -1;
                itemData->nImage = -1;

                menuItemInfo.fType |= MFT_OWNERDRAW;
                menuItemInfo.dwItemData = (ULONG_PTR)itemData;

                ATLVERIFY(popupMenu.SetMenuItemInfo(i, TRUE, &menuItemInfo));
            }
        }
    }
};

typedef CInformerMenuT<false>       CInformerMenuHandle;
typedef CInformerMenuT<true>        CInformerMenu;
