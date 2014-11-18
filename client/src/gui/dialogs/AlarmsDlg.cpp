//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/AlarmsDlg.h>
#include <informer/utility/BindDialogHandler.h>


// CIdleHandler

BOOL CAlarmsDlg::OnIdle()
{
    DoDataExchange(DDX_SAVE, IDC_NICKNAME);

    m_strNickName.TrimLeft();
    m_strNickName.TrimRight();

    UIEnable(IDC_ADD, !m_strNickName.IsEmpty());
    UIEnable(IDC_CLEAR, m_ctrlUserList.GetCount() > 0);
    UIEnable(IDC_REMOVE, m_ctrlUserList.GetCurSel() >= 0);

    UIUpdateChildWindows();

    return FALSE;
}

// Event handlers

BOOL CAlarmsDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    UIAddChildWindowContainer(m_hWnd);

    m_ctrlNickName.SetLimitText(32);

    chat::factory::storage().on_users_updated(
        BindDialogHandler(&CAlarmsDlg::OnUsersUpdated));

    m_ctrlUserList.Assign(chat::factory::storage().users().get_alarm_users());

    return TRUE;
}

void CAlarmsDlg::OnClose()
{
    DestroyWindow();
}

void CAlarmsDlg::OnAdd(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    if(!m_strNickName.IsEmpty())
    {
        std::wstring nickname = m_strNickName;
        chat::factory::storage().add_alarm(nickname);

        m_strNickName.Empty();
        m_ctrlNickName.SetFocus();

        DoDataExchange(DDX_LOAD, IDC_NICKNAME);
    }
}

void CAlarmsDlg::OnRemove(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    int itemID = m_ctrlUserList.GetCurSel();

    if(LB_ERR != itemID)
    {
        //chat::chat_user_list const& users = m_ctrlUserList.GetUserList();
        //chat::factory::storage().remove_alarm(users[itemID].nickname());
    }
}

void CAlarmsDlg::OnClear(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    chat::factory::storage().clear_alarms();
}

void CAlarmsDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}

// Storage event handlers

void CAlarmsDlg::OnUsersUpdated(chat::chat_user_cache const& users)
{
    m_ctrlUserList.Assign(users.get_alarm_users());
}
