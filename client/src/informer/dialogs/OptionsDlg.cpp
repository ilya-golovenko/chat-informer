//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/OptionsDlg.h>
#include <informer/common/Config.h>


// Events handlers

BOOL COptionsDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);

    CenterWindow();
    return TRUE;
}

void COptionsDlg::OnClose()
{
    DestroyWindow();
}

// Storage event handlers
