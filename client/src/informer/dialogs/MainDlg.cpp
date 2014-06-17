//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

#include <informer/dialogs/MainDlg.h>
#include <informer/resources/resource.h>
#include <informer/common/Logging.h>
#include <informer/utility/LoginChatJob.h>
#include <informer/utility/BindDialogHandler.h>
#include <core/common.hpp>
#include <core/factory.hpp>
#include <core/chat_user_cache.hpp>
#include <utility/file_utils.hpp>


// Event handlers

BOOL CMainDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    LOG_DEBUG("initializing dialog");

    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    DlgResize_Init(false, false);

    // Remove unnecessary system menu items
    RemoveSystemMenuItem(SC_MAXIMIZE);
    RemoveSystemMenuItem(SC_MINIMIZE);
    RemoveSystemMenuItem(SC_RESTORE);

    // Setup application icons

    CIconHandle iconBig;
    CIconHandle iconSmall;

    iconBig.LoadIcon(IDR_MAINFRAME, ::GetSystemMetrics(SM_CXICON),
        ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR | LR_SHARED);

    iconSmall.LoadIcon(IDR_MAINFRAME, ::GetSystemMetrics(SM_CXSMICON),
        ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR | LR_SHARED);

    SetIcon(iconBig, TRUE);
    SetIcon(iconSmall, FALSE);

    // Initialize window
    SetupDialogSizeAndPos();
    SetupDialogButtons();
    CreateMainMenu();
    RegisterHotkeys();

    // Initialize notify icon
    CreateNotifyIcon();
    UpdateNotifyIconText();

    // Sort user list
    m_ctrlUserList.Sort(cfg::common::sort_by_nicknames);

    // Subscribe to necessary events
    m_eventManager.OnEventAdded(BindDialogHandler(&CMainDlg::OnEventAdded));
    m_eventManager.OnEventRemoved(BindDialogHandler(&CMainDlg::OnEventRemoved));
    m_informerManager.OnStateChanged(BindDialogHandler(&CMainDlg::OnStateChanged));

    missio::factory::storage().on_users_updated(BindDialogHandler(&CMainDlg::OnUsersUpdated));

    return TRUE;
}

void CMainDlg::OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo)
{
    lpMinMaxInfo->ptMinTrackSize.x = 150;
    lpMinMaxInfo->ptMaxTrackSize.x = 300;
    lpMinMaxInfo->ptMinTrackSize.y = 150;
}

void CMainDlg::OnShowWindow(BOOL bShow, UINT /*nStatus*/)
{
    SetMsgHandled(FALSE);

    if(!bShow)
    {
        SetProcessWorkingSetSize(GetCurrentProcess(),
            static_cast<SIZE_T>(-1), static_cast<SIZE_T>(-1));
    }
}

void CMainDlg::OnSysCommand(UINT nID, CPoint /*pt*/)
{
    if(SC_CLOSE == nID)
        ShowWindow(SW_HIDE);
    else
        SetMsgHandled(FALSE);
}

void CMainDlg::OnClose()
{
    DestroyWindow();
}

void CMainDlg::OnDestroy()
{
    LOG_DEBUG("destroying dialog");

    SetMsgHandled(FALSE);

    // save user list sort type
    cfg::common::sort_by_nicknames = m_ctrlUserList.IsSortedByNicknames();

    // Unregister system wide hotkeys
    UnregisterHotkeys();

    // Uninstall notify icon
    m_notifyIcon.Uninstall();

    // Quit
    PostQuitMessage(0);
}

void CMainDlg::OnMove(CPoint /*pt*/)
{
    SetMsgHandled(FALSE);

    CRect rcWindow;
    GetWindowRect(rcWindow);

    cfg::windows::main::top = rcWindow.top;
    cfg::windows::main::left = rcWindow.left;
}

void CMainDlg::OnSize(UINT nType, CSize /*size*/)
{
    SetMsgHandled(FALSE);

    if(SIZE_MINIMIZED != nType)
    {
        CRect rcWindow;
        GetWindowRect(rcWindow);

        cfg::windows::main::width = rcWindow.Width();
        cfg::windows::main::height = rcWindow.Height();
    }
}

void CMainDlg::OnHotKey(int nHotKeyID, UINT /*uModifiers*/, UINT /*uVirtKey*/)
{
    LOG_DEBUG("received hot key (ID: ", nHotKeyID, ")");

    switch(nHotKeyID)
    {
        case ID_HOTKEY_MAIN:
            m_dialogManager.SwitchDialogVisibility(IDD_MAIN);
            break;

        case ID_HOTKEY_FORUM:
            m_dialogManager.SwitchDialogVisibility(IDD_FORUM);
            m_eventManager.RemoveEvent(missio::event::forum);
            break;

        case ID_HOTKEY_PHOTO:
            m_dialogManager.SwitchDialogVisibility(IDD_PHOTOALBUM);
            m_eventManager.RemoveEvent(missio::event::photoalbum);
            break;

        case ID_HOTKEY_NEWS:
            m_dialogManager.SwitchDialogVisibility(IDD_NEWS);
            m_eventManager.RemoveEvent(missio::event::news);
            break;

        default:
            SetMsgHandled(FALSE);
    }
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent)
    {
        case ID_TIMER_ICON:
            BlinkNotifyIcon();
            break;

        default:
            SetMsgHandled(FALSE);
    }
}

LRESULT CMainDlg::OnCloseMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    LOG_DEBUG("close message received");

    DestroyWindow();
    bHandled = TRUE;

    return 0L;
}

// Notify icon events

void CMainDlg::NotifyIcon_OnLButtonUp(UINT /*nMsg*/)
{
    if(!m_eventManager.ContainsEvents())
    {
        m_dialogManager.SwitchDialogVisibility(IDD_MAIN);
    }
    else
    {
        missio::event::type event = m_eventManager.GetEvent();
        m_dialogManager.ShowDialog(CEventManager::GetEventDialogID(event));
    }
}

void CMainDlg::NotifyIcon_OnRButtonUp(UINT /*nMsg*/)
{
    CPoint pt;
    ::GetCursorPos(&pt);
    ShowMainMenu(pt);
}

// Menu events

void CMainDlg::OnShowHide(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.SwitchDialogVisibility(IDD_MAIN);
}

void CMainDlg::OnForum(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_FORUM);
    m_eventManager.RemoveEvent(missio::event::forum);
}

void CMainDlg::OnPhotoAlbum(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_PHOTOALBUM);
    m_eventManager.RemoveEvent(missio::event::photoalbum);
}

void CMainDlg::OnNews(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_NEWS);
    m_eventManager.RemoveEvent(missio::event::news);
}

void CMainDlg::OnBirthdays(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_BIRTHDAYS);
}

void CMainDlg::OnSearchUser(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_SEARCHUSER);
}

void CMainDlg::OnLinks(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_LINKS);
}

void CMainDlg::OnAlarms(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_ALARMS);
}

void CMainDlg::OnAuthorization(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_LOGIN);
}

void CMainDlg::OnLogin(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_asyncJobManager.AddJob(CLoginChatJob::Create());
}
/*
void CMainDlg::OnIconDownloaded(missio::download::completion_event_args const& args)
{
    if(args.error)
    {
    }
}
*/
void CMainDlg::OnOptions(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    // Implement!
    //m_dialogManager.ShowDialog(IDD_OPTIONS);

    //DEBUG!
    //m_dialogManager.ShowDialog(IDD_CONNECTION);

    //m_soundManager.Play(0);

    //DEBUG!
    //m_dialogManager.ShowNotification("This is a test notification message!", IDD_INVALID);

    //net::http::download::pointer download = missio::download::create(
    //    net::http::uri("http://spchat.ru/favicon.ico"),
    //    boost::filesystem::path("c:\\favicon.ico"),
    //    boost::bind(&OnIconDownloaded, _1));

    //missio::factory::downloader().add_download(download); 
}

void CMainDlg::OnAbout(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_ABOUT);
}

void CMainDlg::OnExit(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}

void CMainDlg::OnMenu(UINT /*uNotifyCode*/, int /*nID*/, HWND hWnd)
{
    CRect rc;
    ::GetWindowRect(hWnd, rc);

    CPoint pt = rc.TopLeft();
    pt.Offset(0, -4);

    ShowMainMenu(pt);
}

// Storage event handlers

void CMainDlg::OnUsersUpdated(missio::chat_user_cache const& users)
{
    m_ctrlUserList.Assign(users.get_online_users());
    UpdateNotifyIconText();
}

// Informer manager event handlers

void CMainDlg::OnStateChanged(InformerState state)
{
    LOG_DEBUG("informer state changed: ", state);

    UpdateNotifyIcon();
    UpdateNotifyIconText();

    if(InformerOnline == state)
    {
        SetMainMenuItemEnabled(IDC_FORUM, TRUE);
        SetMainMenuItemEnabled(IDC_PHOTOALBUM, TRUE);
        SetMainMenuItemEnabled(IDC_BIRTHDAYS, TRUE);
        SetMainMenuItemEnabled(IDC_NEWS, TRUE);
        SetMainMenuItemEnabled(IDC_LINKS, TRUE);
        SetMainMenuItemEnabled(IDC_SEARCHUSER, TRUE);
    }
}

// Event manager event handlers

void CMainDlg::OnEventAdded(missio::event::type event)
{
    LOG_DEBUG("new event added: ", event);

    m_eventManager.ShowEventNotification(event);

    switch(event)
    {
        case missio::event::version:
            SetMainMenuItemImage(IDC_ABOUT, ICON_YELLOW, OVERLAY_NEW);
            break;

        case missio::event::forum:
            SetMainMenuItemImage(IDC_FORUM, ICON_FORUM, OVERLAY_NEW);
            break;

        case missio::event::photoalbum:
            SetMainMenuItemImage(IDC_PHOTOALBUM, ICON_PHOTO, OVERLAY_NEW);
            break;

        case missio::event::news:
            SetMainMenuItemImage(IDC_NEWS, ICON_NEWS, OVERLAY_NEW);
            break;
    }

    SetNotifyIconBlinking(true);
    UpdateNotifyIconText();
}

void CMainDlg::OnEventRemoved(missio::event::type event)
{
    LOG_DEBUG("new removed: ", event);

    switch(event)
    {
        case missio::event::forum:
            SetMainMenuItemImage(IDC_FORUM, ICON_FORUM, OVERLAY_NONE);
            break;

        case missio::event::photoalbum:
            SetMainMenuItemImage(IDC_PHOTOALBUM, ICON_PHOTO, OVERLAY_NONE);
            break;

        case missio::event::news:
            SetMainMenuItemImage(IDC_NEWS, ICON_NEWS, OVERLAY_NONE);
            break;
    }

    if(!m_eventManager.ContainsEvents())
        SetNotifyIconBlinking(false);

    UpdateNotifyIconText();
}

// Implementation

void CMainDlg::RegisterHotkeys()
{
    LOG_DEBUG("regitering hot keys");

    ::RegisterHotKey(m_hWnd, ID_HOTKEY_MAIN, cfg::hotkeys::main::modifiers, cfg::hotkeys::main::virtual_key);
    ::RegisterHotKey(m_hWnd, ID_HOTKEY_FORUM, cfg::hotkeys::forum::modifiers, cfg::hotkeys::forum::virtual_key);
    ::RegisterHotKey(m_hWnd, ID_HOTKEY_PHOTO, cfg::hotkeys::photos::modifiers, cfg::hotkeys::photos::virtual_key);
    ::RegisterHotKey(m_hWnd, ID_HOTKEY_NEWS, cfg::hotkeys::news::modifiers, cfg::hotkeys::news::virtual_key);
}

void CMainDlg::UnregisterHotkeys()
{
    LOG_DEBUG("unregistering hot keys");

    ::UnregisterHotKey(m_hWnd, ID_HOTKEY_MAIN);
    ::UnregisterHotKey(m_hWnd, ID_HOTKEY_FORUM);
    ::UnregisterHotKey(m_hWnd, ID_HOTKEY_PHOTO);
    ::UnregisterHotKey(m_hWnd, ID_HOTKEY_NEWS);
}

void CMainDlg::SetupDialogSizeAndPos()
{
    LOG_DEBUG("setting dialog size and position");

    MoveWindow(cfg::windows::main::left, cfg::windows::main::top,
        cfg::windows::main::width, cfg::windows::main::height, TRUE);
}

void CMainDlg::SetupDialogButtons()
{
    LOG_DEBUG("configuring dialog buttons");

    CString strTip;

    strTip.LoadString(IDS_SHOW_FORUM);
    m_btnEvents.SetToolTipText(strTip);

    strTip.LoadString(IDS_SHOW_MENU);
    m_btnMenu.SetToolTipText(strTip);

    m_btnEvents.SetImages(ICON_FORUM);
    m_btnMenu.SetImages(ICON_YELLOW);
}

void CMainDlg::CreateNotifyIcon()
{
    LOG_DEBUG("creating notify icon");

    m_notifyIcon.AddIcon(m_iconManager.GetIcon(ICON_YELLOW));
    m_notifyIcon.AddIcon(m_iconManager.GetIcon(ICON_GREEN));
    m_notifyIcon.AddIcon(m_iconManager.GetIcon(ICON_RED));

    m_notifyIcon.Install(m_hWnd, ICON_YELLOW);
}

void CMainDlg::UpdateNotifyIcon()
{
    LOG_DEBUG("updating notify icon");

    switch(m_informerManager.GetState())
    {
        case InformerOnline:
            m_notifyIcon.Change(ICON_GREEN);
            break;

        case InformerStandby:
            m_notifyIcon.Change(ICON_YELLOW);
            break;

        case InformerOffline:
            m_notifyIcon.Change(ICON_RED);
            break;
    }
}

void CMainDlg::UpdateNotifyIconText()
{
    LOG_DEBUG("updating notify icon text");

    CString strEvent;
    CString strCaption;

    if(m_informerManager.IsOnline())
    {
        missio::chat_user_cache const& users = missio::factory::storage().users();
        strCaption.Format(IDS_CHAT_USERS, users.get_online_users().size());
    }
    else if(m_informerManager.IsOffline())
    {
        strCaption.LoadString(IDS_CONNECT_ERROR);
    }
    else
    {
        strCaption.LoadString(IDS_CONNECTING);
    }

    SetWindowText(strCaption);

    if(m_eventManager.ContainsEvents())
    {
        missio::event::type event = m_eventManager.PeekEvent();
        strEvent.LoadString(CEventManager::GetEventStringID(event));
    }

    if(!strEvent.IsEmpty())
    {
        strCaption += L"\n";
        strCaption += strEvent;
    }

    m_notifyIcon.SetTip(strCaption);
}

void CMainDlg::SetNotifyIconBlinking(bool blink)
{
    LOG_DEBUG("setting notify icon blinking: ", blink);

    if(blink)
    {
        //m_notifyIcon.ChangeIcon(NULL);
        //SetTimer(ID_TIMER_ICON, 600);
    }
    else
    {
        //UpdateNotifyIcon();
        //KillTimer(ID_TIMER_ICON);
    }
}

void CMainDlg::BlinkNotifyIcon()
{
/*
    int icon = m_notifyIcon.GetCurrentIcon();

    if(icon == ICON_HOLLOW)
        UpdateNotifyIcon();
    else
        m_notifyIcon.ChangeIcon(ICON_HOLLOW);
*/
}

void CMainDlg::CreateMainMenu()
{
    LOG_DEBUG("creating main menu");

    ATLVERIFY(m_mainMenu.LoadMenu(IDR_MAINMENU));
    CInformerMenuHandle menu = m_mainMenu.GetSubMenu(0);

    menu.SetMenuItemImage(IDC_WINDOW, FALSE, ICON_WINDOW, OVERLAY_UP);
    menu.SetMenuItemImage(IDC_FORUM, FALSE, ICON_FORUM, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_PHOTOALBUM, FALSE, ICON_PHOTO, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_NEWS, FALSE, ICON_NEWS, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_BIRTHDAYS, FALSE, ICON_BIRTHDAY, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_SEARCHUSER, FALSE, ICON_MALE, OVERLAY_SEARCH);
    menu.SetMenuItemImage(IDC_LINKS, FALSE, ICON_LINKS, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_AUTH, FALSE, ICON_AUTH, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_LOGIN, FALSE, ICON_LOGIN, OVERLAY_UP);
    menu.SetMenuItemImage(IDC_ALARMS, FALSE, ICON_ALARM, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_OPTIONS, FALSE, ICON_OPTIONS, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_ABOUT, FALSE, ICON_ABOUT, OVERLAY_NONE);
    menu.SetMenuItemImage(IDC_EXIT, FALSE, ICON_EXIT, OVERLAY_NONE);

    menu.SetMenuDefaultItem(IDC_WINDOW);
}

void CMainDlg::ShowMainMenu(CPoint pt)
{
    if(::SetForegroundWindow(m_hWnd))
    {
        if(IsWindowVisible())
        {
            SetMainMenuItemString(IDC_WINDOW, IDS_HIDE);
            SetMainMenuItemImage(IDC_WINDOW, ICON_WINDOW, OVERLAY_DOWN);
        }
        else
        {
            SetMainMenuItemString(IDC_WINDOW, IDS_SHOW);
            SetMainMenuItemImage(IDC_WINDOW, ICON_WINDOW, OVERLAY_UP);
        }

        if(m_informerManager.IsBadCredentials())
        {
            SetMainMenuItemImage(IDC_AUTH, ICON_AUTH, OVERLAY_ERROR);
            SetMainMenuItemEnabled(IDC_LOGIN, FALSE);
        }
        else if(!m_informerManager.HasCredentials())
        {
            SetMainMenuItemImage(IDC_AUTH, ICON_AUTH, OVERLAY_ATTENTION);
            SetMainMenuItemEnabled(IDC_LOGIN, FALSE);
        }
        else
        {
            SetMainMenuItemImage(IDC_AUTH, ICON_AUTH, OVERLAY_NONE);
            SetMainMenuItemEnabled(IDC_LOGIN, TRUE);
        }

        SetMainMenuDefaultItem(IDC_WINDOW);

        m_mainMenu.GetSubMenu(0).TrackPopupMenuEx(
            TPM_LEFTALIGN | TPM_BOTTOMALIGN,
            pt.x, pt.y, m_hWnd);

        PostMessage(WM_NULL);
    }
}

void CMainDlg::SetMainMenuItemEnabled(UINT itemID, BOOL enabled)
{
    UINT enable = enabled ? MF_ENABLED : MF_DISABLED;
    m_mainMenu.GetSubMenu(0).EnableMenuItem(itemID, enable);
}

void CMainDlg::SetMainMenuItemString(UINT itemID, UINT stringID)
{
    CString strTemp(reinterpret_cast<LPCWSTR>(stringID));
    CInformerMenuHandle menu = m_mainMenu.GetSubMenu(0);
    menu.SetMenuItemString(itemID, FALSE, strTemp);
}

void CMainDlg::SetMainMenuDefaultItem(UINT itemID)
{
    m_mainMenu.GetSubMenu(0).SetMenuDefaultItem(itemID);
}

void CMainDlg::SetMainMenuItemImage(UINT itemID, int icon, int overlay)
{
    CInformerMenuHandle menu = m_mainMenu.GetSubMenu(0);
    menu.SetMenuItemImage(itemID, FALSE, icon, overlay);
}
