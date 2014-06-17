//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

#include <informer/dialogs/SendMessageDlg.h>
#include <informer/common/Logging.h>
#include <core/server_errors.hpp>


// Operations

void CSendMessageDlg::SetUserNickName(CString const& strNickName)
{
    m_strNickName = strNickName;
    DoDataExchange(DDX_LOAD, IDC_NICKNAME);
}

// CIdleHandler

BOOL CSendMessageDlg::OnIdle()
{
    DoDataExchange(DDX_SAVE, IDC_MESSAGE);

    m_strMessage.TrimLeft();
    m_strMessage.TrimRight();

    UIEnable(IDC_SEND, !m_strMessage.IsEmpty());
    UIUpdateChildWindows();

    return FALSE;
}

// Event handlers

BOOL CSendMessageDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    UIAddChildWindowContainer(m_hWnd);

    m_ctrlMessage.SetLimitText(255);

    return TRUE;
}

void CSendMessageDlg::OnClose()
{
    DestroyWindow();
}

void CSendMessageDlg::OnDestroy()
{
    SetMsgHandled(FALSE);

    if(m_query)
    {
        m_query->cancel();
        m_query.reset();
    }
}

void CSendMessageDlg::OnSend(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    if(!m_strMessage.IsEmpty())
    {
        m_btnSend.EnableWindow(FALSE);
        m_ctrlMessage.EnableWindow(FALSE);

        std::wstring nickname = m_strNickName;
        std::wstring message = m_strMessage;

        boost::algorithm::replace_all(message, L"\r", L"");
        boost::algorithm::replace_all(message, L"\n", L" ");
        boost::algorithm::replace_all(message, L"\t", L" ");

        json::value json_data;

        json_data["message"]["text"] = message;
        json_data["message"]["nickname"] = nickname;

        m_query = missio::query::create(json_data, true,
            boost::bind(&CSendMessageDlg::OnSendMessage, this, _1));

        missio::factory::informer().add_query(m_query);
    }
}

void CSendMessageDlg::OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}

void CSendMessageDlg::OnKeyDown_Message(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
    SetMsgHandled(FALSE);

    if(VK_RETURN == nChar)
    {
        SHORT keyState = GetKeyState(VK_CONTROL);

        if(keyState & 0x8000)
        {
            SetMsgHandled(TRUE);
            m_btnSend.Click();
        }
    }
}

// Informer event handlers

void CSendMessageDlg::OnSendMessage(missio::query::pointer query)
{
    if(IsWindow())
    {
        if(!query->error())
        {
            try
            {
                json::object_cref json_data = query->json_data();
                json::object_cref json_result = json_data["send_result"];

                if(!json_result["success"].as<bool>())
                {
                    switch(json_result["error"].as<int>())
                    {
                        case missio::server_error::bad_credentials:
                            m_dialogManager.ShowMessage(IDS_SEND_CREDENTIALS);
                            cfg::chat::bad_credentials = true;
                            break;

                        case missio::server_error::send_denied:
                            m_dialogManager.ShowMessage(IDS_SEND_DENIED);
                            break;

                        case missio::server_error::db_connect:
                        case missio::server_error::db_query:
                            m_dialogManager.ShowMessage(IDS_SEND_SERVER);
                            break;

                        case missio::server_error::should_wait:
                            m_dialogManager.ShowMessage(IDS_SEND_WAIT);
                            break;

                        case missio::server_error::ignored:
                            m_dialogManager.ShowMessage(IDS_SEND_IGNORED);
                            break;

                        case missio::server_error::offline:
                            m_dialogManager.ShowMessage(IDS_SEND_OFFLINE);
                            break;

                        case missio::server_error::generic:
                            m_dialogManager.ShowMessage(IDS_SEND_FAILURE);
                            break;

                        default:
                            m_dialogManager.ShowMessage(IDS_SEND_FAILURE);
                    }
                }
            }
            catch(std::exception const& e)
            {
                LOG_ERROR("caught exception: ", e);
            }
        }
        else
        {
            m_dialogManager.ShowMessage(IDS_SEND_FAILURE);
        }

        DestroyWindow();
    }
}

// Implementation

CSendMessageDlg::CSendMessageDlg() :
    m_ctrlMessage(this, 1)
{
}
