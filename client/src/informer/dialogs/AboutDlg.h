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
#include <core/factory.hpp>


class CAboutDlg :
    public CInformerDlgImpl<CAboutDlg>,
    public CWinDataExchange<CAboutDlg>
{
public:
    enum
    {
        ID_TIMER_TRAFFIC
    };

public:
    // Controls
    CInformerButton m_btnOK;
    CInformerButton m_btnUpdate;
    CInformerFrame m_ctrlMainFrame;

    // Members
    WTL::CString m_strInformerName;
    WTL::CString m_strOutputTraffic;
    WTL::CString m_strInputTraffic;

public:
    DIALOG_RESOURCE_ID(IDD_ABOUT)

    // Message map and event handlers

    BEGIN_MSG_MAP(CAboutDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_TIMER(OnTimer)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDC_UPDATE, OnUpdate)
        CHAIN_MSG_MAP(CInformerDlgImpl<CAboutDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CAboutDlg)
        DDX_CONTROL(IDOK, m_btnOK)
        DDX_CONTROL(IDC_UPDATE, m_btnUpdate)
        DDX_CONTROL(IDC_MAINFRAME, m_ctrlMainFrame)
        DDX_TEXT(IDC_INFORMERNAME, m_strInformerName)
        DDX_TEXT(IDC_OUTPUTTRAFFIC, m_strOutputTraffic)
        DDX_TEXT(IDC_INPUTTRAFFIC, m_strInputTraffic)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnTimer(UINT_PTR nIDEvent);
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnUpdate(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);

    // Implementation
    CAboutDlg();
    void UpdateInformerVersion();
    void UpdateTrafficCounters();
    void UpdateTrafficCounter(CString& strCounter, double lfCounter);

private:
    std::size_t m_lastBytesRead;
    std::size_t m_lastBytesWritten;
};
