//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

#include <informer/dialogs/ForumDlg.h>
#include <informer/utility/BindDialogHandler.h>


// Events handlers

BOOL CForumDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    DlgResize_Init(false, false);

    // Remove unnecessary system menu items
    RemoveSystemMenuItem(SC_MAXIMIZE);
    RemoveSystemMenuItem(SC_MINIMIZE);
    RemoveSystemMenuItem(SC_RESTORE);

    // Set window size and position
    SetupDialogSizeAndPos();

    missio::factory::storage().on_forum_updated(
        BindDialogHandler(&CForumDlg::OnForumUpdated));

    m_ctrlForumTopicList.Assign(missio::factory::storage().forum().topics());

    return TRUE;
}

void CForumDlg::OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo)
{
    lpMinMaxInfo->ptMinTrackSize.x = 250;
    lpMinMaxInfo->ptMinTrackSize.y = 100;
}

void CForumDlg::OnShowWindow(BOOL bShow, UINT /*nStatus*/)
{
    SetMsgHandled(FALSE);

    if(FALSE == bShow)
    {
        m_ctrlForumTopicList.MarkTopicsAsOld();

        ::SetProcessWorkingSetSize(GetCurrentProcess(),
            static_cast<SIZE_T>(-1), static_cast<SIZE_T>(-1));
    }
}

void CForumDlg::OnSysCommand(UINT nID, CPoint /*pt*/)
{
    if(SC_CLOSE == nID)
        ShowWindow(SW_HIDE);
    else
        SetMsgHandled(FALSE);
}

void CForumDlg::OnClose()
{
    DestroyWindow();
}

void CForumDlg::OnMove(CPoint /*pt*/)
{
    SetMsgHandled(FALSE);

    CRect rcWindow;
    GetWindowRect(rcWindow);

    cfg::windows::forum::top = rcWindow.top;
    cfg::windows::forum::left = rcWindow.left;
}

void CForumDlg::OnSize(UINT nType, CSize /*size*/)
{
    SetMsgHandled(FALSE);

    if(SIZE_MINIMIZED != nType)
    {
        CRect rcWindow;
        GetWindowRect(rcWindow);

        cfg::windows::forum::width = rcWindow.Width();
        cfg::windows::forum::height = rcWindow.Height();
    }
}

// Storage event handlers

void CForumDlg::OnForumUpdated(missio::forum const& forum)
{
    m_ctrlForumTopicList.Assign(forum.topics());
}

// Implementation

void CForumDlg::SetupDialogSizeAndPos()
{
    MoveWindow(cfg::windows::forum::left, cfg::windows::forum::top,
        cfg::windows::forum::width, cfg::windows::forum::height, TRUE);
}
