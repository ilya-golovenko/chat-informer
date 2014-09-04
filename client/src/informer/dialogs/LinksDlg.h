//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

#include <informer/resources/resource.h>
#include <informer/controls/InformerDialog.h>
#include <informer/controls/LinkListCtrl.h>
#include <core/factory.hpp>


class CLinksDlg :
    public CInformerDlgImpl<CLinksDlg>,
    public CDialogResize<CLinksDlg>,
    public CWinDataExchange<CLinksDlg>
{
public:
    // Controls
    CLinkListCtrl m_ctrlLinks;

public:
    DIALOG_RESOURCE_ID(IDD_LINKS)

    // Message map and events handlers

    BEGIN_MSG_MAP(CLinksDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
        MSG_WM_CLOSE(OnClose)
        CHAIN_MSG_MAP(CInformerDlgImpl<CLinksDlg>)
        CHAIN_MSG_MAP(CDialogResize<CLinksDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(CLinksDlg)
        DLGRESIZE_CONTROL(IDC_LINKLIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

    BEGIN_DDX_MAP(CLinksDlg)
        DDX_CONTROL(IDC_LINKLIST, m_ctrlLinks)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo);
    void OnClose();
    void OnSize(UINT /*nType*/, CSize size);

    // Storage event handlers
    void OnLinksUpdated(chat::link_list const& links);
};
