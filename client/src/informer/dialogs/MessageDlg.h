//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/controls/InformerDialog.h>
#include <informer/controls/InformerButton.h>
#include <informer/controls/InformerStaticText.h>


class CMessageDlg :
    public CInformerDlgImpl<CMessageDlg>,
    public CWinDataExchange<CMessageDlg>
{
public:
    // Controls
    CInformerStaticText m_ctrlMessage;
    CInformerButton m_btnOK;

    // Members
    CString m_strMessage;

public:
    DIALOG_RESOURCE_ID(IDD_MESSAGE)

    // Message map and event handlers

    BEGIN_MSG_MAP(CMessageDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        CHAIN_MSG_MAP(CInformerDlgImpl<CMessageDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CMessageDlg)
        DDX_TEXT(IDC_MESSAGE, m_strMessage)
        DDX_CONTROL(IDC_MESSAGE, m_ctrlMessage)
        DDX_CONTROL(IDOK, m_btnOK)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);

    // Implementation
    CMessageDlg(CString const& strMessage);
};
