//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/ConnectionDlg.h>


// CIdleHandler

BOOL CConnectionDlg::OnIdle()
{
    DoDataExchange(DDX_SAVE, IDC_PRECONFIG);
    DoDataExchange(DDX_SAVE, IDC_PROXYAUTH);

    BOOL bEnable = USE_PROXY == m_proxyUsage;

    UIEnable(IDC_PROXYNAME, bEnable);
    UIEnable(IDC_PROXYPORT, bEnable);
    UIEnable(IDC_PROXYAUTH, bEnable);

    bEnable = bEnable && m_proxyAuth;

    UIEnable(IDC_USERNAME, bEnable);
    UIEnable(IDC_PASSWORD, bEnable);

    UIUpdateChildWindows();

    return FALSE;
}

// Event handlers

BOOL CConnectionDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    UIAddChildWindowContainer(m_hWnd);

    return TRUE;
}

void CConnectionDlg::OnClose()
{
    DestroyWindow();
}

void CConnectionDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    if(DoDataExchange(DDX_SAVE))
    {


        DestroyWindow();
    }
}

void CConnectionDlg::OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}

// Implementation

CConnectionDlg::CConnectionDlg() :
    m_proxyUsage(0),
    m_proxyAuth(false),
    m_proxyPort(3128)
{
}
