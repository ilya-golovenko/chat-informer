//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/LoginDlg.h>


// Event handlers

BOOL CLoginDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    //m_strNickName = m_informerManager.GetNickName();
    //m_strPassword = m_informerManager.GetPassword();

    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);

    m_ctrlNickName.SetLimitText(32);
    m_ctrlPassword.SetLimitText(32);

    return TRUE;
}

void CLoginDlg::OnClose()
{
    DestroyWindow();
}

void CLoginDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DoDataExchange(DDX_SAVE);

    if(m_ctrlNickName.GetModify()
        || m_ctrlPassword.GetModify())
    {
        m_strNickName.TrimLeft();
        m_strNickName.TrimRight();

        if(m_strNickName.IsEmpty())
            m_strPassword.Empty();

        std::wstring nickname = m_strNickName;
        std::wstring password = m_strPassword;

        m_informerManager.UpdateCredentials(nickname, password);
    }

    DestroyWindow();
}

void CLoginDlg::OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}
