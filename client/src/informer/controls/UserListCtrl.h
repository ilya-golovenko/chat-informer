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
    #error UserListCtrl.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/resources/resource.h>
#include <informer/controls/InformerMenu.h>
#include <informer/controls/InformerListCtrl.h>
#include <core/chat_user_list.hpp>


template
<
    class T,
    class TBase = CWindow,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CUserListCtrlImpl :
    public CInformerListCtrlImpl<T, TBase, TWinTraits>
{
public:
    // Controls
    CInformerMenu m_sortMenu;
    CInformerMenu m_userMenu;
    CInformerMenu m_listMenu;

public:
    enum
    {
        MAX_ICONS = 3
    };

    enum
    {
        LIST_WIDTH_SPACING  = 4,
        LIST_HEIGHT_SPACING = 2
    };

public:
    static UINT const drawTextFormat = DT_NOCLIP |
        DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    void Assign(chat::chat_user_list const& users)
    {
        std::wstring nickname;
        int curSel = GetCurSel();

        if(LB_ERR != curSel)
            nickname = m_users[curSel].nickname();

        m_users.assign(users);
        m_users.sort(m_sortUsersByNicknames);

        if(LB_ERR != curSel)
            curSel = m_users.index_of(nickname);

        BeginUpdate();
        SetCount(m_users.size());
        SetCurSel(curSel);
        EndUpdate();
    }

    void Add(chat::chat_user const& user)
    {
        //TODO: implement
    }

    void Remove(std::wstring const& nickname)
    {
        //TODO: implement
    }

    void Clear()
    {
        m_users.clear();
        ResetContent();
    }

    void Sort(bool sortByNicknames)
    {
        m_sortUsersByNicknames = sortByNicknames;

        SetSortMenuItemChecked(IDC_SORTBYNICK, m_sortUsersByNicknames);
        SetSortMenuItemChecked(IDC_SORTBYSEX, !m_sortUsersByNicknames);

        Assign(m_users);
    }

    bool IsSortedByNicknames() const
    {
        return m_sortUsersByNicknames;
    }

    // Message map and event handlers

    BEGIN_MSG_MAP(CUserListCtrlImpl)
        MSG_WM_CONTEXTMENU(OnContextMenu)
        MSG_WM_COMMAND(OnCommand)
        MSG_OCM_COMMAND(OnCommand)
        CHAIN_MSG_MAP_MEMBER(m_userMenu)
        CHAIN_MSG_MAP_MEMBER(m_listMenu)
        CHAIN_MSG_MAP(CInformerListCtrlImpl)
    END_MSG_MAP()

    void OnContextMenu(HWND /*hWnd*/, CPoint pt)
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            CRect rc;
            GetItemRect(itemID, rc);
            ClientToScreen(rc);

            if(pt.x < 0)
                pt.x = rc.left;

            pt.y = rc.bottom + 2;
        }
        else
        {
            ::GetCursorPos(&pt);
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
                case IDC_USERINFO:
                    ShowCurrentUserInfo();
                    break;

                case IDC_MESSAGE:
                    OpenSendMessageDialog();
                    break;

                case IDC_ALARM:
                    SetCurrentUserAlarm();
                    break;

                case IDC_SORTBYNICK:
                case IDC_SORTBYSEX:
                    Sort(IDC_SORTBYNICK == nID);
                    break;

                default:
                    SetMsgHandled(FALSE);
            }
        }
        else if(LBN_DBLCLK == uNotifyCode)
        {
            ShowCurrentUserInfo();
        }
    }

    // Implementation

    CUserListCtrlImpl() :
        m_sortUsersByNicknames(true)
    {
    }

    void Init()
    {
        CInformerListCtrlImpl<T, TBase, TWinTraits>::Init();

        // Create user popup menu

        ATLVERIFY(m_userMenu.LoadMenu(IDR_USERMENU));
        CInformerMenuHandle userMenu = m_userMenu.GetSubMenu(0);

        userMenu.SetMenuItemImage(IDC_USERINFO, FALSE, ICON_INFO, OVERLAY_NONE);
        userMenu.SetMenuItemImage(IDC_MESSAGE, FALSE, ICON_MESSAGE, OVERLAY_NONE);
        userMenu.SetMenuItemImage(IDC_SORT, FALSE, ICON_MALE, OVERLAY_DOWN);
        userMenu.SetMenuDefaultItem(IDC_USERINFO);

        // Create list popup menu

        ATLVERIFY(m_listMenu.LoadMenu(IDR_USERLISTMENU));
        CInformerMenuHandle listMenu = m_listMenu.GetSubMenu(0);
        listMenu.SetMenuItemImage(IDC_SORT, FALSE, ICON_MALE, OVERLAY_DOWN);

        // Create sort popup menu

        ATLVERIFY(m_sortMenu.LoadMenu(IDR_SORTMENU));
        m_sortMenu.CheckMenuItem(IDC_SORTBYNICK, MF_CHECKED);

        // Append sort menu to user and list menus

        CMenuItemInfo menuItemInfo;
        menuItemInfo.fMask = MIIM_SUBMENU;
        menuItemInfo.hSubMenu = m_sortMenu.GetSubMenu(0);

        userMenu.SetMenuItemInfo(IDC_SORT, FALSE, &menuItemInfo);
        listMenu.SetMenuItemInfo(IDC_SORT, FALSE, &menuItemInfo);
    }
    
    void ShowPopupMenu(CPoint pt)
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            chat::chat_user const& user = m_users[itemID];

            if(chat::chat_user::guest != user.sex())
                SetUserMenuItemEnabled(IDC_USERINFO, TRUE);
            else
                SetUserMenuItemEnabled(IDC_USERINFO, FALSE);

            if(user.online() &&
                m_informerManager.HasCredentials() &&
                !m_informerManager.IsBadCredentials())
            {
                SetUserMenuItemEnabled(IDC_MESSAGE, TRUE);
            }
            else
            {
                SetUserMenuItemEnabled(IDC_MESSAGE, FALSE);
            }

            if(user.alarm())
            {
                SetUserMenuItemImage(IDC_ALARM, ICON_ALARM, OVERLAY_REMOVE);
                SetUserMenuItemString(IDC_ALARM, IDS_REMOVE_ALARM);
            }
            else
            {
                SetUserMenuItemImage(IDC_ALARM, ICON_ALARM, OVERLAY_ADD);
                SetUserMenuItemString(IDC_ALARM, IDS_ADD_ALARM);
            }

            m_userMenu.GetSubMenu(0).TrackPopupMenuEx(
                TPM_TOPALIGN, pt.x, pt.y, m_hWnd);
        }
        else
        {
            m_listMenu.GetSubMenu(0).TrackPopupMenuEx(
                TPM_TOPALIGN, pt.x, pt.y, m_hWnd);
        }
    }

    void SetUserMenuItemEnabled(UINT itemID, BOOL enabled)
    {
        UINT enable = enabled ? MF_ENABLED : MF_DISABLED;
        m_userMenu.GetSubMenu(0).EnableMenuItem(itemID, enable);
    }

    void SetSortMenuItemChecked(UINT itemID, BOOL checked)
    {
        UINT check = checked ? MF_CHECKED : MF_UNCHECKED;
        m_sortMenu.GetSubMenu(0).CheckMenuItem(itemID, check);
    }

    void SetUserMenuItemString(UINT itemID, UINT stringID)
    {
        CString strTemp(reinterpret_cast<LPCWSTR>(stringID));
        CInformerMenuHandle menu = m_userMenu.GetSubMenu(0);
        menu.SetMenuItemString(itemID, FALSE, strTemp);
    }

    void SetUserMenuItemImage(UINT itemID, int icon, int overlay)
    {
        CInformerMenuHandle menu = m_userMenu.GetSubMenu(0);
        menu.SetMenuItemImage(itemID, FALSE, icon, overlay);
    }

    void OpenSendMessageDialog()
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            chat::chat_user const& user = m_users[itemID];
            m_dialogManager.SendMessage(user.nickname().c_str());
        }
    }

    void ShowCurrentUserInfo()
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            chat::chat_user const& user = m_users[itemID];

            if(chat::chat_user::guest == user.sex())
                m_dialogManager.ShowMessage(IDS_NOT_REGISTERED);
            else
                m_dialogManager.ShowUserInfo(user.nickname().c_str());
        }
    }

    void SetCurrentUserAlarm()
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            chat::chat_user const& user = m_users[itemID];
            chat::factory::storage().switch_alarm(user.nickname());
        }
    }

    // CInformerListCtrlImpl

    void MeasureItem(UINT itemID, CDCHandle dc, CSize& itemSize)
    {
        CRect rcText;

        chat::chat_user const& user = m_users[itemID];

        CFontHandle newFont = m_drawManager.Font(FONT_MAIN);
        CFontHandle oldFont = dc.SelectFont(newFont);

        dc.DrawText(
            user.nickname().c_str(),
            user.nickname().size(),
            rcText,
            drawTextFormat | DT_CALCRECT);

        TEXTMETRIC tm;
        dc.GetTextMetrics(&tm);

        dc.SelectFont(oldFont);

        LONG iconCX = ::GetSystemMetrics(SM_CXSMICON);
        LONG iconCY = ::GetSystemMetrics(SM_CYSMICON);

        itemSize.cx = iconCX + rcText.Width() + LIST_WIDTH_SPACING;
        itemSize.cy = max(iconCY, tm.tmHeight) + LIST_HEIGHT_SPACING;
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

        int icon—ount = 0;
        int icons[MAX_ICONS];

        chat::chat_user const& user = m_users[itemID];

        if(chat::chat_user::unknown != user.sex())
        {
            int customIcon = m_iconManager.FindCustomIconIndex(user.nickname());

            if(customIcon >= 0)
            {
                icons[icon—ount++] = customIcon;
            }
            else
            {
                switch(user.sex())
                {
                    case chat::chat_user::guest:
                        icons[icon—ount++] = ICON_GUEST;
                        break;

                    case chat::chat_user::hidden:
                        icons[icon—ount++] = ICON_HIDDEN;
                        break;

                    case chat::chat_user::male:
                        icons[icon—ount++] = ICON_MALE;
                        break;

                    case chat::chat_user::female:
                        icons[icon—ount++] = ICON_FEMALE;
                        break;
                }
            }
        }

        if(user.birthday())
            icons[icon—ount++] = ICON_BIRTHDAY;

        if(user.alarm())
            icons[icon—ount++] = ICON_ALARM;

        int iconCX = ::GetSystemMetrics(SM_CXSMICON);
        int iconCY = ::GetSystemMetrics(SM_CYSMICON);

        CPoint pt(2, rcItem.top + (rcItem.Height() - iconCY) / 2);
        rcItem.OffsetRect(4 + (2 + iconCX) * icon—ount, 0);

        for(int index = 0; index < icon—ount; ++index, pt.x += 2 + iconCX)
            m_iconManager.DrawIcon(dc, pt, icons[index], OVERLAY_NONE, NORMAL_ICON);

        dc.SetBkMode(TRANSPARENT);

        dc.SelectFont(m_drawManager.Font(FONT_MAIN));
        dc.SetTextColor(m_drawManager.Color(textColor));

        dc.DrawText(
            user.nickname().c_str(),
            user.nickname().size(),
            rcItem,
            drawTextFormat);
    }

    void DrawBackground(CDCHandle dc, CRect const& rcClient)
    {
        DrawFillRect(dc, rcClient, BRUSH_LIGHT);
    }

private:
    bool m_sortUsersByNicknames;
    chat::chat_user_list m_users;
};

class CUserListCtrl :
    public CUserListCtrlImpl<CUserListCtrl>
{
public:
    DECLARE_WND_SUPERCLASS(L"UserListCtrl", GetWndClassName())
};
