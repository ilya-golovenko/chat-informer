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
    #error NotifyIcon.h requires atlwin.h to be included first
#endif

#if (_WIN32_IE < 0x0400)
    #error NotifyIcon.h requires _WIN32_IE >= 0x0400
#endif


#define MSG_NOTIFYICON_HANDLER(notifyIcon, msg, func) \
    if(uMsg == notifyIcon.m_notifyIconData.uCallbackMessage && \
        lParam == msg && wParam == notifyIcon.m_notifyIconData.uID) \
    { \
        SetMsgHandled(TRUE); \
        func((UINT)lParam); \
        lResult = 0; \
        if(IsMsgHandled()) \
            return TRUE; \
    }


struct CNotifyIconData : NOTIFYICONDATA
{
    CNotifyIconData()
    {
        ::ZeroMemory(this, sizeof(NOTIFYICONDATA));
        cbSize = sizeof(NOTIFYICONDATA);
    }
};

class ATL_NO_VTABLE CNotifyIcon
{
public:
    enum { WM_NOTIFY_ICON = WM_APP + 2004 };

public:
    HWND m_hWnd;
    UINT m_icon;

    std::vector<HICON> m_icons;

    UINT m_taskbarRestartMessage;
    CNotifyIconData m_notifyIconData;

public:
    // Operations

    UINT AddIcon(HICON hIcon)
    {
        ATLASSERT(hIcon != NULL);

        m_icons.push_back(hIcon);
        return m_icons.size() - 1;
    }

    void ClearIconList()
    {
        boost::for_each(m_icons, boost::bind<BOOL>(&::DestroyIcon, _1));

        m_icons.clear();
    }

    UINT GetIconCount() const
    {
        return m_icons.size();
    }

    HICON GetIcon(UINT icon) const
    {
        ATLASSERT(icon < m_icons.size());

        return m_icons[icon];
    }

    UINT GetCurrentIcon() const
    {
        return m_icon;
    }

    BOOL Install(HWND hWnd, UINT icon)
    {
        ATLASSERT(::IsWindow(hWnd));
        ATLASSERT(icon < m_icons.size());

        if(IsInstalled())
            return FALSE;

        m_hWnd = hWnd;
        m_icon = icon;

        m_notifyIconData.uFlags = NIF_MESSAGE | NIF_ICON;
        m_notifyIconData.hIcon = m_icons[m_icon];
        m_notifyIconData.hWnd = m_hWnd;

        return ::Shell_NotifyIcon(NIM_ADD, &m_notifyIconData);
    }

    BOOL Change(UINT icon)
    {
        ATLASSERT(icon < m_icons.size());

        if(!IsInstalled())
            return FALSE;

        m_icon = icon;

        m_notifyIconData.hIcon = m_icons[m_icon];
        m_notifyIconData.uFlags = NIF_ICON;
        m_notifyIconData.hWnd = m_hWnd;

        return ::Shell_NotifyIcon(NIM_MODIFY, &m_notifyIconData);
    }

    BOOL SetTip(LPCWSTR lpszTip = NULL)
    {
        if(!IsInstalled())
            return FALSE;

        m_notifyIconData.uFlags = NIF_TIP;
        m_notifyIconData.hWnd = m_hWnd;

        int maxLength = sizeof(m_notifyIconData.szTip) / sizeof(WCHAR);
        ::lstrcpyn(m_notifyIconData.szTip, lpszTip, maxLength);
        m_notifyIconData.szTip[maxLength - 1] = L'\0';

        return ::Shell_NotifyIcon(NIM_MODIFY, &m_notifyIconData);
    }

    BOOL Uninstall()
    {
        if(!IsInstalled())
            return FALSE;

        m_hWnd = NULL;

        return ::Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);
    }

    BOOL IsInstalled() const
    {
        return NULL != m_hWnd;
    }

    // Implementation

    CNotifyIcon() :
        m_hWnd(NULL),
        m_icon(0)
    {
        m_notifyIconData.uCallbackMessage = WM_NOTIFY_ICON;
        m_taskbarRestartMessage = ::RegisterWindowMessage(L"TaskbarCreated");
    }

    ~CNotifyIcon()
    {
        Uninstall();
        ClearIconList();
    }

    // Message map and handlers

    BEGIN_MSG_MAP(CNotifyIcon)
        MESSAGE_HANDLER(m_taskbarRestartMessage, OnTaskbarRestart)
    END_MSG_MAP()

    LRESULT OnTaskbarRestart(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        HWND hWnd = m_hWnd;
        m_hWnd = NULL;

        Install(hWnd, m_icon);
        SetTip(m_notifyIconData.szTip);

        bHandled = FALSE;
        return 0L;
    }
};
