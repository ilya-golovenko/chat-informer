//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/resources/resource.h>
#include <informer/controls/InformerDialog.h>
#include <informer/controls/ForumTopicListCtrl.h>
#include <core/factory.hpp>


class CForumDlg :
    public CInformerDlgImpl<CForumDlg>,
    public CDialogResize<CForumDlg>,
    public CWinDataExchange<CForumDlg>
{
public:
    // Controls
    CForumTopicListCtrl m_ctrlForumTopicList;

public:
    DIALOG_RESOURCE_ID(IDD_FORUM)

    // Message map and events handlers

    BEGIN_MSG_MAP(CForumDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_MOVE(OnMove)
        MSG_WM_SIZE(OnSize)
        CHAIN_MSG_MAP(CInformerDlgImpl<CForumDlg>)
        CHAIN_MSG_MAP(CDialogResize<CForumDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(CForumDlg)
        DLGRESIZE_CONTROL(IDC_FORUMTOPICLIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

    BEGIN_DDX_MAP(CForumDlg)
        DDX_CONTROL(IDC_FORUMTOPICLIST, m_ctrlForumTopicList)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo);
    void OnShowWindow(BOOL bShow, UINT /*nStatus*/);
    void OnSysCommand(UINT nID, CPoint /*pt*/);
    void OnClose();
    void OnMove(CPoint /*pt*/);
    void OnSize(UINT /*nType*/, CSize /*size*/);

    // Storage event handlers
    void OnForumUpdated(missio::forum const& forum);

    // Implementation
    void SetupDialogSizeAndPos();
};
