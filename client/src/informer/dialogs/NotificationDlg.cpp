//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/NotificationDlg.h>


// Operations

void CNotificationDlg::SetNotification(CString const& strText, int dialogID)
{
    m_alpha = MIN_ALPHA;
    m_strText = strText;
    m_dialogID = dialogID;
    DoDataExchange(DDX_LOAD);
    StartTimer(ID_TIMER_SHOWING);
}

// Event handlers

BOOL CNotificationDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    ModifyStyleEx(0, WS_EX_LAYERED);

    SetupDialogPosition();
    SetWindowAlpha(m_alpha);

    return TRUE;
}

void CNotificationDlg::OnSysCommand(UINT nID, CPoint /*pt*/)
{
    if(SC_CLOSE == nID)
    {
        KillAllTimers();
        ShowWindow(SW_HIDE);
    }
    else
    {
        SetMsgHandled(FALSE);
    }
}

void CNotificationDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent)
    {
        case ID_TIMER_SHOWING:
            if(m_alpha + ALPHA_INCREMENT <= MAX_ALPHA)
            {
                m_alpha += ALPHA_INCREMENT;
                SetWindowAlpha(m_alpha);
            }
            else
            {
                StartTimer(ID_TIMER_VISIBLE);
            }
            break;

        case ID_TIMER_VISIBLE:
            StartTimer(ID_TIMER_HIDING);
            break;

        case ID_TIMER_HIDING:
            if(m_alpha - ALPHA_INCREMENT >= 0)
            {
                m_alpha -= ALPHA_INCREMENT;
                SetWindowAlpha(m_alpha);
            }
            else
            {
                KillAllTimers();
                ShowWindow(SW_HIDE);
            }
            break;

        default:
            SetMsgHandled(FALSE);
    }
}

void CNotificationDlg::OnMove(CPoint /*pt*/)
{
    SetMsgHandled(FALSE);

    CRect rcWindow;
    GetWindowRect(rcWindow);

    cfg::windows::notification::top = rcWindow.top;
    cfg::windows::notification::left = rcWindow.left;
}

void CNotificationDlg::OnLButtonDown(UINT /*nFlags*/, CPoint/*pt*/)
{
    KillAllTimers();
    ShowWindow(SW_HIDE);

    if(IDD_INVALID != m_dialogID)
        m_dialogManager.ShowDialog(m_dialogID);

    //ReleaseCapture();
    //SendMessage(WM_NCLBUTTONDOWN, HTCAPTION);
}

void CNotificationDlg::OnRButtonDown(UINT /*nFlags*/, CPoint/*pt*/)
{
    KillAllTimers();
    ShowWindow(SW_HIDE);
}

// CMouseTrackHelper

void CNotificationDlg::HandleMouseEnter(CPoint /*pt*/)
{
    KillAllTimers();
    SetWindowAlpha(255);
}

void CNotificationDlg::HandleMouseLeave()
{
    m_alpha = MAX_ALPHA;
    SetWindowAlpha(m_alpha);
    StartTimer(ID_TIMER_VISIBLE);
}

// Implementation

CNotificationDlg::CNotificationDlg() :
    m_alpha(MIN_ALPHA),
    m_dialogID(IDD_INVALID)
{
}

void CNotificationDlg::SetupDialogPosition()
{
    SetWindowPos(
        0, cfg::windows::notification::left, cfg::windows::notification::top,
        0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void CNotificationDlg::SetWindowAlpha(BYTE alpha)
{
    ::SetLayeredWindowAttributes(m_hWnd, CLR_INVALID, alpha, LWA_ALPHA);
}

void CNotificationDlg::StartTimer(UINT_PTR timerID)
{
    switch(timerID)
    {
        case ID_TIMER_SHOWING:
            SetTimer(ID_TIMER_SHOWING, SHOWING_INTERVAL, 0);
            break;

        case ID_TIMER_VISIBLE:
            KillTimer(ID_TIMER_SHOWING);
            SetTimer(ID_TIMER_VISIBLE, VISIBLE_INTERVAL, 0);
            break;

        case ID_TIMER_HIDING:
            KillTimer(ID_TIMER_VISIBLE);
            SetTimer(ID_TIMER_HIDING, HIDING_INTERVAL, 0);
            break;
    }
}

void CNotificationDlg::KillAllTimers()
{
    KillTimer(ID_TIMER_SHOWING);
    KillTimer(ID_TIMER_VISIBLE);
    KillTimer(ID_TIMER_HIDING);
}
