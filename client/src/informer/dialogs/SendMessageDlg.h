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
#include <core/factory.hpp>
#include <core/query.hpp>


class CSendMessageDlg :
    public CInformerDlgImpl<CSendMessageDlg>,
    public CWinDataExchange<CSendMessageDlg>,
    public CUpdateUI<CSendMessageDlg>
{
public:
    // Controls
    CInformerButton m_btnSend;
    CInformerButton m_btnCancel;
    CContainedWindowT<CEdit> m_ctrlMessage;

    // Members
    CString m_strNickName;
    CString m_strMessage;

public:
    DIALOG_RESOURCE_ID(IDD_SENDMESSAGE)

    // Operations

    void SetUserNickName(const CString& strNickName);

    // CIdleHandler

    virtual BOOL OnIdle();

    // Message map and event handlers

    BEGIN_MSG_MAP_EX(CSendMessageDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_SEND, OnSend)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        CHAIN_MSG_MAP(CInformerDlgImpl<CSendMessageDlg>)
        REFLECT_NOTIFICATIONS()
    ALT_MSG_MAP(1)
        MSG_WM_KEYDOWN(OnKeyDown_Message)
    END_MSG_MAP()

    BEGIN_DDX_MAP(CSendMessageDlg)
        DDX_CONTROL(IDC_SEND, m_btnSend)
        DDX_CONTROL(IDCANCEL, m_btnCancel)
        DDX_CONTROL(IDC_MESSAGE, m_ctrlMessage)
        DDX_TEXT(IDC_NICKNAME, m_strNickName)
        DDX_TEXT(IDC_MESSAGE, m_strMessage)
    END_DDX_MAP()

    BEGIN_UPDATE_UI_MAP(CSendMessageDlg)
        UPDATE_ELEMENT(IDC_SEND, UPDUI_CHILDWINDOW)
    END_UPDATE_UI_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnDestroy();
    void OnSend(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnKeyDown_Message(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/);

    // Informer event handlers
    void OnSendMessage(missio::query::pointer query);

    // Implementation
    CSendMessageDlg();

private:
    missio::query::pointer m_query;
};
