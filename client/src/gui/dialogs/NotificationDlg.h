//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/controls/InformerDialog.h>
#include <informer/controls/InformerStaticText.h>
#include <informer/utility/MouseTrackHelper.h>
#include <core/events.hpp>


class CNotificationDlg :
    public CInformerDlgImpl<CNotificationDlg>,
    public CWinDataExchange<CNotificationDlg>,
    public CMouseTrackHelper<CNotificationDlg>
{
public:
    enum
    {
        ID_TIMER_SHOWING,
        ID_TIMER_VISIBLE,
        ID_TIMER_HIDING
    };

    enum
    {
        MIN_ALPHA           = 0,
        MAX_ALPHA           = 230,
        ALPHA_INCREMENT     = 10,

        VISIBLE_INTERVAL    = 5000,

        SHOWING_INTERVAL    = 1000 * ALPHA_INCREMENT / MAX_ALPHA,
        HIDING_INTERVAL     = 750 * ALPHA_INCREMENT / MAX_ALPHA
    };

public:
    // Controls
    CInformerStaticText m_ctrlText;

    // Members
    BYTE m_alpha;
    CString m_strText;
    int m_dialogID;

public:
    DIALOG_RESOURCE_ID(IDD_NOTIFICATION)

    // Operations

    void SetNotification(CString const& strText, int dialogID);

    // Message map and event handlers

    BEGIN_MSG_MAP_EX(CNotificationDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_MOVE(OnMove)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_RBUTTONDOWN(OnRButtonDown)
        CHAIN_MSG_MAP(CInformerDlgImpl<CNotificationDlg>)
        CHAIN_MSG_MAP(CMouseTrackHelper<CNotificationDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CNotificationDlg)
        DDX_CONTROL(IDC_TEXT, m_ctrlText)
        DDX_TEXT(IDC_TEXT, m_strText)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnSysCommand(UINT nID, CPoint /*pt*/);
    void OnTimer(UINT_PTR nIDEvent);
    void OnMove(CPoint /*pt*/);
    void OnLButtonDown(UINT /*nFlags*/, CPoint/*pt*/);
    void OnRButtonDown(UINT /*nFlags*/, CPoint/*pt*/);

    // MouseTrackHelper
    void HandleMouseEnter(CPoint /*pt*/);
    void HandleMouseLeave();

    // Implementation
    CNotificationDlg();
    void SetupDialogPosition();
    void SetWindowAlpha(BYTE alpha);
    void StartTimer(UINT_PTR timerID);
    void KillAllTimers();
};
