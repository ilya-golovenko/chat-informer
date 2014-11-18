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
#include <core/factory.hpp>


class COptionsDlg :
    public CInformerDlgImpl<COptionsDlg>,
    public CWinDataExchange<COptionsDlg>
{
public:
    // Controls

public:
    DIALOG_RESOURCE_ID(IDD_OPTIONS)

    // Message map and events handlers

    BEGIN_MSG_MAP(COptionsDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        CHAIN_MSG_MAP(CInformerDlgImpl<COptionsDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(COptionsDlg)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();

    // Storage event handlers
};
