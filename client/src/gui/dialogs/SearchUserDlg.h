//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/controls/InformerButton.h>
#include <informer/controls/InformerDialog.h>


class CSearchUserDlg :
    public CInformerDlgImpl<CSearchUserDlg>,
    public CWinDataExchange<CSearchUserDlg>,
    public CUpdateUI<CSearchUserDlg>
{
public:
    // Controls
    CEdit m_ctrlNickName;
    CInformerButton m_btnOK;
    CInformerButton m_btnCancel;

    // Members
    CString m_strNickName;

public:
    DIALOG_RESOURCE_ID(IDD_SEARCHUSER)

    // CIdleHandler

    virtual BOOL OnIdle();

    // Message map and event handlers

    BEGIN_MSG_MAP_EX(CSearchUserDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        CHAIN_MSG_MAP(CInformerDlgImpl)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CSearchUserDlg)
        DDX_TEXT(IDC_NICKNAME, m_strNickName)
        DDX_CONTROL_HANDLE(IDC_NICKNAME, m_ctrlNickName)
        DDX_CONTROL(IDOK, m_btnOK)
        DDX_CONTROL(IDCANCEL, m_btnCancel)
    END_DDX_MAP()

    BEGIN_UPDATE_UI_MAP(CSearchUserDlg)
        UPDATE_ELEMENT(IDOK, UPDUI_CHILDWINDOW)
    END_UPDATE_UI_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
};
