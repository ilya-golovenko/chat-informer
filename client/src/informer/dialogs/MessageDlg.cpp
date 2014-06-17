//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/MessageDlg.h>


// Event handlers

BOOL CMessageDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    return TRUE;
}

void CMessageDlg::OnClose()
{
    EndDialog(0);
}

void CMessageDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    EndDialog(0);
}

// Implementation

CMessageDlg::CMessageDlg(CString const& strMessage) :
    m_strMessage(strMessage)
{
}
