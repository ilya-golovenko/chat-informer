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


class CLoginDlg :
    public CInformerDlgImpl<CLoginDlg>,
    public CWinDataExchange<CLoginDlg>
{
public:
    // Controls
    WTL::CEdit m_ctrlNickName;
    WTL::CEdit m_ctrlPassword;
    CInformerButton m_btnOK;
    CInformerButton m_btnCancel;

    // Members
    WTL::CString m_strNickName;
    WTL::CString m_strPassword;

public:
    DIALOG_RESOURCE_ID(IDD_LOGIN)

    // Message map and event handlers

    BEGIN_MSG_MAP_EX(CLoginDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        CHAIN_MSG_MAP(CInformerDlgImpl<CLoginDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CLoginDlg)
        DDX_TEXT(IDC_NICKNAME, m_strNickName)
        DDX_TEXT(IDC_PASSWORD, m_strPassword)
        DDX_CONTROL_HANDLE(IDC_NICKNAME, m_ctrlNickName)
        DDX_CONTROL_HANDLE(IDC_PASSWORD, m_ctrlPassword)
        DDX_CONTROL(IDOK, m_btnOK)
        DDX_CONTROL(IDCANCEL, m_btnCancel)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
};
