//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/LinksDlg.h>
#include <informer/utility/BindDialogHandler.h>


// Events handlers

BOOL CLinksDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    DlgResize_Init(false, false);

    CSize idealSize;
    m_ctrlLinks.GetIdealSize(idealSize);
    ResizeClient(idealSize.cx, idealSize.cy);

    missio::factory::storage().on_links_updated(
        BindDialogHandler(&CLinksDlg::OnLinksUpdated));

    m_ctrlLinks.Assign(missio::factory::storage().links());

    CenterWindow();
    return TRUE;
}

void CLinksDlg::OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo)
{
    lpMinMaxInfo->ptMinTrackSize.x = 250;
    lpMinMaxInfo->ptMinTrackSize.y = 100;
}

void CLinksDlg::OnClose()
{
    DestroyWindow();
}

// Storage event handlers

void CLinksDlg::OnLinksUpdated(missio::link_list const& links)
{
    m_ctrlLinks.Assign(links);
}
