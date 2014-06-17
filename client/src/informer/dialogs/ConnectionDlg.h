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


class CConnectionDlg :
    public CInformerDlgImpl<CConnectionDlg>,
    public CWinDataExchange<CConnectionDlg>,
    public CUpdateUI<CConnectionDlg>
{
public:
    // Controls
    CInformerFrame m_ctrlFrame;
    WTL::CEdit m_editProxyName;
    WTL::CEdit m_editProxyPort;
    WTL::CButton m_ctrlProxyAuth;
    WTL::CEdit m_editUserName;
    WTL::CEdit m_editPassword;
    CInformerButton m_btnOK;
    CInformerButton m_btnCancel;

    // Members
    int m_proxyUsage;
    bool m_proxyAuth;
    WTL::CString m_strProxyName;
    unsigned int m_proxyPort;
    WTL::CString m_strUserName;
    WTL::CString m_strPassword;

public:
    enum
    {
        PRECONFIG,
        NO_PROXY,
        USE_PROXY
    };

public:
    DIALOG_RESOURCE_ID(IDD_CONNECTION)

    // CIdleHandler

    virtual BOOL OnIdle();

    // Message map and event handlers

    BEGIN_MSG_MAP(CConnectionDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        CHAIN_MSG_MAP(CInformerDlgImpl<CConnectionDlg>)
        CHAIN_MSG_MAP(CUpdateUI<CConnectionDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CConnectionDlg)
        DDX_CONTROL(IDC_FRAME, m_ctrlFrame)
        DDX_CONTROL_HANDLE(IDC_PROXYNAME, m_editProxyName)
        DDX_CONTROL_HANDLE(IDC_PROXYPORT, m_editProxyPort)
        DDX_CONTROL_HANDLE(IDC_PROXYAUTH, m_ctrlProxyAuth)
        DDX_CONTROL_HANDLE(IDC_USERNAME, m_editUserName)
        DDX_CONTROL_HANDLE(IDC_PASSWORD, m_editPassword)
        DDX_CONTROL(IDOK, m_btnOK)
        DDX_CONTROL(IDCANCEL, m_btnCancel)
        DDX_RADIO(IDC_PRECONFIG, m_proxyUsage)
        DDX_TEXT(IDC_PROXYNAME, m_strProxyName)
        DDX_UINT_RANGE(IDC_PROXYPORT, m_proxyPort, 1U, 65534U)
        DDX_CHECK(IDC_PROXYAUTH, m_proxyAuth)
        DDX_TEXT(IDC_USERNAME, m_strUserName)
        DDX_TEXT(IDC_PASSWORD, m_strPassword)
    END_DDX_MAP()

    BEGIN_UPDATE_UI_MAP(CConnectionDlg)
        UPDATE_ELEMENT(IDC_PRECONFIG, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_NOPROXY, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_USEPROXY, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_PROXYNAME, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_PROXYPORT, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_PROXYAUTH, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_USERNAME, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_PASSWORD, UPDUI_CHILDWINDOW)
    END_UPDATE_UI_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);

    // Implementation
    CConnectionDlg();
};
