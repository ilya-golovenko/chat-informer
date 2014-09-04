//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/controls/InformerDialog.h>
#include <informer/controls/InformerHtmlCtrl.h>
#include <core/factory.hpp>
#include <core/news.hpp>


class CNewsDlg :
    public CInformerDlgImpl<CNewsDlg>,
    public CDialogResize<CNewsDlg>,
    public CWinDataExchange<CNewsDlg>
{
public:
    // Controls
    CInformerHtmlCtrl m_ctrlHtml;

public:
    DIALOG_RESOURCE_ID(IDD_NEWS)

    // Message map and event handlers

    BEGIN_MSG_MAP(CNewsDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        CHAIN_MSG_MAP(CInformerDlgImpl<CNewsDlg>)
        CHAIN_MSG_MAP(CDialogResize<CNewsDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CNewsDlg)
        DDX_CONTROL(IDC_HTMLCTRL, m_ctrlHtml)
    END_DDX_MAP()

    BEGIN_DLGRESIZE_MAP(CNewsDlg)
        DLGRESIZE_CONTROL(IDC_HTMLCTRL, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnDestroy();

    // Storage event handlers
    void OnNewsUpdated(chat::news const& news);

    // Downloader event handlers
    void OnTemplateDownloaded(chat::download::pointer download,
                              boost::system::error_code const& error);

    // Implementation
    void UpdateNews(chat::news const& news);

private:
    chat::download::pointer m_download;
};
