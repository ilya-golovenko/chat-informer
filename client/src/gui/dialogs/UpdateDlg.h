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
#include <informer/controls/InformerProgressCtrl.h>
#include <core/factory.hpp>
#include <core/download.hpp>


class CUpdateDlg :
    public CInformerDlgImpl<CUpdateDlg>,
    public CWinDataExchange<CUpdateDlg>
{
public:
    // Controls
    CInformerStaticText m_ctrlMessage;
    CInformerProgressCtrl m_ctrlProgress;
    CInformerButton m_btnStartUpdate;
    CInformerButton m_btnCancel;

    WTL::CString m_strMessage;

    // Members
    chat::download::pointer m_setup_download;

public:
    DIALOG_RESOURCE_ID(IDD_UPDATE)

    // Message map and event handlers

    BEGIN_MSG_MAP(CUpdateDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_STARTUPDATE, OnStartUpdate)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        CHAIN_MSG_MAP(CInformerDlgImpl<CUpdateDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CUpdateDlg)
        DDX_CONTROL(IDC_MESSAGE, m_ctrlMessage)
        DDX_CONTROL(IDC_PROGRESS, m_ctrlProgress)
        DDX_CONTROL(IDC_STARTUPDATE, m_btnStartUpdate)
        DDX_CONTROL(IDCANCEL, m_btnCancel)
        DDX_TEXT(IDC_MESSAGE, m_strMessage)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnDestroy();
    void OnStartUpdate(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);

    // Downloader events
    //void OnSetupDownloadProgress(chat::download::progress_event_args const& args);
    //void OnSetupDownloadCompleted(chat::download::completion_event_args const& args);

    // Implementation
    void StartDownloadingSetupExecutable();
    void RunSetupExecutable(boost::filesystem::path const& filename);
    void CancelSetupDownload();
    void SetMessage(UINT nResourceID);
    void SetApplicationUpdateFailure();
};
