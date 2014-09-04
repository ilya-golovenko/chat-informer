//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/AboutDlg.h>
#include <core/version.hpp>
#include <core/common.hpp>
#include <net/common/statistics.hpp>

// Event handlers

BOOL CAboutDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    UpdateInformerVersion();
    UpdateTrafficCounters();

    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    SetTimer(ID_TIMER_TRAFFIC, 500);

    chat::storage& storage = chat::factory::storage();
    chat::version const& version = storage.server_version();

    if(chat::version::unknown != version)
    {
        if(version > chat::informer_version)
            m_btnUpdate.ShowWindow(SW_SHOW);
    }

    return TRUE;
}

void CAboutDlg::OnClose()
{
    DestroyWindow();
}

void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
    if(ID_TIMER_TRAFFIC == nIDEvent)
        UpdateTrafficCounters();
    else
        SetMsgHandled(FALSE);
}

void CAboutDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}

void CAboutDlg::OnUpdate(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    m_dialogManager.ShowDialog(IDD_UPDATE);
}

// Implementation

CAboutDlg::CAboutDlg() :
    m_lastBytesRead(~std::size_t(0)),
    m_lastBytesWritten(~std::size_t(0))
{
}

void CAboutDlg::UpdateInformerVersion()
{
    DoDataExchange(DDX_SAVE, IDC_INFORMERNAME);

    chat::version version = chat::informer_version;
    std::wstring version_string = version.to_wstring();

    m_strInformerName += version_string.c_str();
    DoDataExchange(DDX_LOAD, IDC_INFORMERNAME);
}

void CAboutDlg::UpdateTrafficCounters()
{
    std::size_t bytesWritten = net::statistics::bytes_written();
    std::size_t bytesRead = net::statistics::bytes_read();

    if(bytesWritten != m_lastBytesWritten)
    {
        UpdateTrafficCounter(m_strOutputTraffic, bytesWritten);
        DoDataExchange(DDX_LOAD, IDC_OUTPUTTRAFFIC);
    }

    if(bytesRead != m_lastBytesRead)
    {
        UpdateTrafficCounter(m_strInputTraffic, bytesRead);
        DoDataExchange(DDX_LOAD, IDC_INPUTTRAFFIC);
    }

    m_lastBytesWritten = bytesWritten;
    m_lastBytesRead = bytesRead;
}

void CAboutDlg::UpdateTrafficCounter(CString& strCounter, double lfCounter)
{
    static UINT formats[] =
    {
        IDS_TRAFFIC_GBYTES,
        IDS_TRAFFIC_MBYTES,
        IDS_TRAFFIC_KBYTES,
        IDS_TRAFFIC_BYTES
    };

    int index = sizeof(formats) / sizeof(*formats) - 1;

    while(index && lfCounter / 1024 > 1)
    {
        lfCounter /= 1024;
        --index;
    }

    strCounter.Format(formats[index], lfCounter);
}
