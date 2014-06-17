//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

#include <informer/dialogs/BirthdaysDlg.h>
#include <informer/utility/BindDialogHandler.h>
#include <core/chat_user_cache.hpp>


// Event handlers

BOOL CBirthdaysDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);

    missio::factory::storage().on_users_updated(
        BindDialogHandler(&CBirthdaysDlg::OnUsersUpdated));

    SetWindowCaption(missio::factory::storage().users().get_birthdays_today());
    m_ctrlUserList.Assign(missio::factory::storage().users().get_birthday_users());

    return TRUE;
}

void CBirthdaysDlg::OnClose()
{
    DestroyWindow();
}

void CBirthdaysDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}

// Storage event handlers

void CBirthdaysDlg::OnUsersUpdated(missio::chat_user_cache const& users)
{
    SetWindowCaption(users.get_birthdays_today());
    m_ctrlUserList.Assign(users.get_birthday_users());
}

// Implementation

void CBirthdaysDlg::SetWindowCaption(std::wstring const& today)
{
    CString strText;

    if(!today.empty())
    {
        strText.Format(IDS_BIRTHDAYS, today.c_str());
    }
    else
    {
        CString strUnknown(reinterpret_cast<LPCWSTR>(IDS_UNKNOWN));
        strText.Format(IDS_BIRTHDAYS, static_cast<LPCWSTR>(strUnknown));
    }

    SetWindowText(strText);
}
