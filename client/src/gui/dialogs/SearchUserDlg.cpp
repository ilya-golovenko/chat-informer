//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/SearchUserDlg.h>


// CIdleHandler

BOOL CSearchUserDlg::OnIdle()
{
    DoDataExchange(DDX_SAVE);

    m_strNickName.TrimLeft();
    m_strNickName.TrimRight();

    UIEnable(IDOK, !m_strNickName.IsEmpty());
    UIUpdateChildWindows();

    return FALSE;
}

// Event handlers

BOOL CSearchUserDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    UIAddChildWindowContainer(m_hWnd);

    m_ctrlNickName.SetLimitText(32);

    return TRUE;
}

void CSearchUserDlg::OnClose()
{
    DestroyWindow();
}

void CSearchUserDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    if(!m_strNickName.IsEmpty())
    {
        m_dialogManager.ShowUserInfo(m_strNickName);
        DestroyWindow();
    }
}

void CSearchUserDlg::OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}
