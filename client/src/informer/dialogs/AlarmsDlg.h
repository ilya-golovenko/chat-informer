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
#include <informer/controls/InformerFrame.h>
#include <informer/controls/UserListCtrl.h>
#include <core/factory.hpp>


class CAlarmsDlg :
    public CInformerDlgImpl<CAlarmsDlg>,
    public CWinDataExchange<CAlarmsDlg>,
    public CUpdateUI<CAlarmsDlg>
{
public:
    // Controls
    WTL::CEdit m_ctrlNickName;
    CInformerFrame m_ctrlFrame;
    CUserListCtrl m_ctrlUserList;
    CInformerButton m_btnAdd;
    CInformerButton m_btnRemove;
    CInformerButton m_btnClear;
    CInformerButton m_btnOK;

    // Members
    WTL::CString m_strNickName;

public:
    DIALOG_RESOURCE_ID(IDD_ALARMS)

    // CIdleHandler

    virtual BOOL OnIdle();

    // Message map and event handlers

    BEGIN_MSG_MAP_EX(CAlarmsDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDC_ADD, OnAdd)
        COMMAND_ID_HANDLER_EX(IDC_REMOVE, OnRemove)
        COMMAND_ID_HANDLER_EX(IDC_CLEAR, OnClear)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        CHAIN_MSG_MAP(CInformerDlgImpl<CAlarmsDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CAlarmsDlg)
        DDX_TEXT(IDC_NICKNAME, m_strNickName)
        DDX_CONTROL_HANDLE(IDC_NICKNAME, m_ctrlNickName)
        DDX_CONTROL(IDC_FRAME, m_ctrlFrame)
        DDX_CONTROL(IDC_USERLIST, m_ctrlUserList)
        DDX_CONTROL(IDC_ADD, m_btnAdd)
        DDX_CONTROL(IDC_REMOVE, m_btnRemove)
        DDX_CONTROL(IDC_CLEAR, m_btnClear)
        DDX_CONTROL(IDOK, m_btnOK)
    END_DDX_MAP()

    BEGIN_UPDATE_UI_MAP(CAlarmsDlg)
        UPDATE_ELEMENT(IDC_ADD, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_REMOVE, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_CLEAR, UPDUI_CHILDWINDOW)
    END_UPDATE_UI_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnAdd(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnRemove(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnClear(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);

    // Storage event handlers
    void OnUsersUpdated(missio::chat_user_cache const& users);
};
