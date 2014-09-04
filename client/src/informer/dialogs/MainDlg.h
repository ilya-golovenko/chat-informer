//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/resources/resource.h>
#include<informer/controls/InformerDialog.h>
#include <informer/controls/InformerBitmapButton.h>
#include <informer/controls/InformerMenu.h>
#include <informer/controls/UserListCtrl.h>
#include <informer/controls/NotifyIcon.h>
#include <core/factory.hpp>
#include <core/query.hpp>


class CMainDlg :
    public CInformerDlgImpl<CMainDlg>,
    public CDialogResize<CMainDlg>,
    public CWinDataExchange<CMainDlg>
{
public:
    enum
    {
        ID_TIMER_ICON
    };

    enum
    {
        ID_HOTKEY_MAIN,
        ID_HOTKEY_FORUM,
        ID_HOTKEY_PHOTO,
        ID_HOTKEY_NEWS,
    };

public:
    // Controls
    CUserListCtrl m_ctrlUserList;
    CInformerBitmapButton m_btnEvents;
    CInformerBitmapButton m_btnMenu;
    CNotifyIcon m_notifyIcon;
    CInformerMenu m_mainMenu;

public:
    DIALOG_RESOURCE_ID(IDD_MAIN)

    // Message map and event handlers

    BEGIN_MSG_MAP(CMainDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_MOVE(OnMove)
        MSG_WM_SIZE(OnSize)
        MSG_WM_HOTKEY(OnHotKey)
        MSG_WM_TIMER(OnTimer)
        COMMAND_ID_HANDLER_EX(IDC_WINDOW, OnShowHide)
        COMMAND_ID_HANDLER_EX(IDC_FORUM, OnForum)
        COMMAND_ID_HANDLER_EX(IDC_PHOTOALBUM, OnPhotoAlbum)
        COMMAND_ID_HANDLER_EX(IDC_NEWS, OnNews)
        COMMAND_ID_HANDLER_EX(IDC_BIRTHDAYS, OnBirthdays)
        COMMAND_ID_HANDLER_EX(IDC_SEARCHUSER, OnSearchUser)
        COMMAND_ID_HANDLER_EX(IDC_LINKS, OnLinks)
        COMMAND_ID_HANDLER_EX(IDC_ALARMS, OnAlarms)
        COMMAND_ID_HANDLER_EX(IDC_AUTH, OnAuthorization)
        COMMAND_ID_HANDLER_EX(IDC_LOGIN, OnLogin)
        COMMAND_ID_HANDLER_EX(IDC_OPTIONS, OnOptions)
        COMMAND_ID_HANDLER_EX(IDC_ABOUT, OnAbout)
        COMMAND_ID_HANDLER_EX(IDC_EXIT, OnExit)
        COMMAND_ID_HANDLER_EX(IDC_MENU, OnMenu)
        MSG_NOTIFYICON_HANDLER(m_notifyIcon, WM_LBUTTONUP, NotifyIcon_OnLButtonUp)
        MSG_NOTIFYICON_HANDLER(m_notifyIcon, WM_RBUTTONUP, NotifyIcon_OnRButtonUp)
        CHAIN_MSG_MAP(CInformerDlgImpl<CMainDlg>)
        CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
        CHAIN_MSG_MAP_MEMBER(m_notifyIcon)
        CHAIN_MSG_MAP_MEMBER(m_mainMenu)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(CMainDlg)
        DLGRESIZE_CONTROL(IDC_USERLIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_MENU, DLSZ_MOVE_Y | DLSZ_SIZE_X)
        DLGRESIZE_CONTROL(IDC_FORUM, DLSZ_MOVE_Y)
    END_DLGRESIZE_MAP()

    BEGIN_DDX_MAP(CMainDlg)
        DDX_CONTROL(IDC_USERLIST, m_ctrlUserList)
        DDX_CONTROL(IDC_FORUM, m_btnEvents)
        DDX_CONTROL(IDC_MENU, m_btnMenu)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo);
    void OnShowWindow(BOOL bShow, UINT /*nStatus*/);
    void OnSysCommand(UINT nID, CPoint /*pt*/);
    void OnClose();
    void OnDestroy();
    void OnMove(CPoint /*pt*/);
    void OnSize(UINT /*nType*/, CSize /*size*/);
    void OnHotKey(int nHotKeyID, UINT /*uModifiers*/, UINT /*uVirtKey*/);
    void OnTimer(UINT_PTR nIDEvent);
    LRESULT OnCloseMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

    // Notify icon events

    void NotifyIcon_OnLButtonUp(UINT /*nMsg*/);
    void NotifyIcon_OnRButtonUp(UINT /*nMsg*/);

    // Menu events

    void OnShowHide(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnForum(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnPhotoAlbum(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnNews(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnBirthdays(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnSearchUser(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnLinks(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnAlarms(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnAuthorization(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnLogin(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnOptions(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnAbout(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnExit(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnMenu(UINT /*uNotifyCode*/, int /*nID*/, HWND hWnd);

    // Storage event handlers
    void OnUsersUpdated(chat::chat_user_cache const& users);

    // EventManager event handlers
    void OnEventAdded(chat::event::type event);
    void OnEventRemoved(chat::event::type event);

    // Informer manager event handlers
    void OnStateChanged(InformerState state);

    // Implementation
    void RegisterHotkeys();
    void UnregisterHotkeys();
    void SetupDialogSizeAndPos();
    void SetupDialogButtons();
    void CreateNotifyIcon();
    void UpdateNotifyIcon();
    void UpdateNotifyIconText();
    void SetNotifyIconBlinking(bool blink);
    void BlinkNotifyIcon();
    void CreateMainMenu();
    void ShowMainMenu(CPoint pt);
    void SetMainMenuItemEnabled(UINT itemID, BOOL enabled);
    void SetMainMenuItemString(UINT itemID, UINT stringID);
    void SetMainMenuDefaultItem(UINT itemID);
    void SetMainMenuItemImage(UINT itemID, int icon, int overlay);
};
