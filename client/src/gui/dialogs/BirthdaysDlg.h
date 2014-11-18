//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/controls/InformerDialog.h>
#include <informer/controls/InformerFrame.h>
#include <informer/controls/InformerButton.h>
#include <informer/controls/UserListCtrl.h>
#include <core/factory.hpp>


class CBirthdaysDlg :
    public CInformerDlgImpl<CBirthdaysDlg>,
    public CWinDataExchange<CBirthdaysDlg>
{
public:
    // Controls
    CUserListCtrl m_ctrlUserList;
    CInformerFrame m_ctrlFrame;
    CInformerButton m_btnOK;

public:
    DIALOG_RESOURCE_ID(IDD_BIRTHDAYS)

    // Message map and event handlers

    BEGIN_MSG_MAP(CBirthdaysDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        CHAIN_MSG_MAP(CInformerDlgImpl<CBirthdaysDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CBirthdaysDlg)
        DDX_CONTROL(IDC_USERLIST, m_ctrlUserList)
        DDX_CONTROL(IDC_FRAME, m_ctrlFrame)
        DDX_CONTROL(IDOK, m_btnOK)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);

    // Storage event handlers
    void OnUsersUpdated(chat::chat_user_cache const& users);

    // Implementation
    void SetWindowCaption(std::wstring const& today);
};
