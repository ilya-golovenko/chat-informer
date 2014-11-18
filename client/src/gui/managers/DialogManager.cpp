//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/DialogManager.h>
#include <informer/dialogs/MainDlg.h>
#include <informer/dialogs/ForumDlg.h>
#include <informer/dialogs/AlarmsDlg.h>
#include <informer/dialogs/BirthdaysDlg.h>
#include <informer/dialogs/LinksDlg.h>
#include <informer/dialogs/SearchUserDlg.h>
#include <informer/dialogs/AboutDlg.h>
#include <informer/dialogs/UpdateDlg.h>
#include <informer/dialogs/PhotoAlbumDlg.h>
#include <informer/dialogs/NewsDlg.h>
#include <informer/dialogs/OptionsDlg.h>
#include <informer/dialogs/ConnectionDlg.h>
#include <informer/dialogs/LoginDlg.h>
#include <informer/dialogs/MessageDlg.h>
#include <informer/dialogs/NotificationDlg.h>
#include <informer/dialogs/UserInfoDlg.h>
#include <informer/dialogs/SendMessageDlg.h>


CDialogManager::CDialogManager() :
    m_main_thread_id(0),
    m_message_hook(0)
{
}

CDialogManager::~CDialogManager()
{
    if(nullptr != m_message_hook)
        ::UnhookWindowsHookEx(m_message_hook);
}

void CDialogManager::Initialize()
{
    LOG_COMP_TRACE_FUNCTION(CDialogManager);

    LOG_COMP_INFO(CDialogManager, "initializing");

    m_main_thread_id = ::GetCurrentThreadId();

    m_message_hook = ::SetWindowsHookEx(
        WH_GETMESSAGE, reinterpret_cast<HOOKPROC>(GetMsgProc),
        ModuleHelper::GetModuleInstance(), m_main_thread_id);

    if(!m_message_hook)
        throw std::runtime_error("cannot create message hook");

    LOG_COMP_INFO(CDialogManager, "registering dialogs");

    Register<CForumDlg>();
    Register<CAlarmsDlg>();
    Register<CBirthdaysDlg>();
    Register<CLinksDlg>();
    Register<CSearchUserDlg>();
    Register<CAboutDlg>();
    Register<CUpdateDlg>();
    Register<CPhotoAlbumDlg>();
    Register<CNewsDlg>();
    Register<COptionsDlg>();
    Register<CConnectionDlg>();
    Register<CLoginDlg>();
}

void CDialogManager::Finalize()
{
    LOG_COMP_TRACE_FUNCTION(CDialogManager);

    LOG_COMP_INFO(CDialogManager, "finalizing");

    if(nullptr != m_message_hook)
        ::UnhookWindowsHookEx(m_message_hook);

    m_message_hook = nullptr;
}

ATL::CWindow CDialogManager::CreateMainDialog()
{
    LOG_COMP_INFO(CDialogManager, "creating main dialog");

    dialog_pointer dialog = CMainDlg::Create(nullptr);

    m_dialog_map.emplace(IDD_MAIN, dialog);

    return *dialog;
}

void CDialogManager::DestroyMainDialog()
{
    LOG_COMP_INFO(CDialogManager, "destroying main dialog");
    GetMainDialog().DestroyWindow();
}

ATL::CWindow CDialogManager::GetMainDialog() const
{
    return GetDialog(IDD_MAIN);
}

void CDialogManager::ShowDialog(int dialogID, bool bringToTop)
{
    LOG_COMP_DEBUG(CDialogManager, "showing dialog (ID: ", dialogID, ")");

    dialog_pointer dialog = FindDialog(dialogID);

    if(!dialog)
        dialog = CreateDialog(dialogID);

    if(bringToTop)
    {
        dialog->ShowWindow(SW_SHOW);
        ::SetForegroundWindow(*dialog);
    }
    else
    {
        dialog->ShowWindow(SW_SHOWNOACTIVATE);
    }
}

void CDialogManager::DestroyDialog(int dialogID)
{
    LOG_COMP_DEBUG(CDialogManager, "destroying dialog (ID: ", dialogID, ")");

    ::PostThreadMessage(m_main_thread_id, WM_DESTROY_DIALOG,
        static_cast<WPARAM>(dialogID), reinterpret_cast<LPARAM>(this));
}

void CDialogManager::SwitchDialogVisibility(int dialogID)
{
    LOG_COMP_DEBUG(CDialogManager, "switching dialog visibility (ID: ", dialogID, ")");

    ATL::CWindow dialog = GetDialog(dialogID);

    if(TRUE == dialog.IsWindow())
    {
        if(TRUE == dialog.IsWindowVisible())
        {
            LOG_COMP_DEBUG(CDialogManager, "closing dialog (ID: ", dialogID, ")");

            dialog.PostMessage(WM_SYSCOMMAND, SC_CLOSE);
        }
        else
        {
            LOG_COMP_DEBUG(CDialogManager, "showing dialog (ID: ", dialogID, ")");

            dialog.ShowWindow(SW_SHOW);
            ::SetForegroundWindow(dialog);
        }
    }
    else
    {
        ShowDialog(dialogID);
    }
}

bool CDialogManager::IsDialogVisible(int dialogID) const
{
    ATL::CWindow dialog = GetDialog(dialogID);

    if(TRUE == dialog.IsWindow())
        return TRUE == dialog.IsWindowVisible();

    return false;
}

ATL::CWindow CDialogManager::GetDialog(int dialogID) const
{
    dialog_map::const_iterator it = m_dialog_map.find(dialogID);
    return it != m_dialog_map.end() ? *it->second : ATL::CWindow(nullptr);
}

void CDialogManager::ShowUserInfo(ATL::CString const& strNickName)
{
    LOG_COMP_DEBUG(CDialogManager, "showing user info dialog (nickname: ", strNickName, ")");
    Show<CUserInfoDlg>()->LoadUserInfo(strNickName);
}

void CDialogManager::SendMessage(ATL::CString const& strNickName)
{
    LOG_COMP_DEBUG(CDialogManager, "showing send message dialog (nickname: ", strNickName, ")");
    Show<CSendMessageDlg>()->SetUserNickName(strNickName);
}

void CDialogManager::ShowMessage(UINT stringID, HWND hWndParent)
{
    ATL::CString strMessage;

    if(strMessage.LoadString(stringID))
        ShowMessage(strMessage, hWndParent);
}

void CDialogManager::ShowMessage(ATL::CString const& strMessage, HWND hWndParent)
{
    LOG_COMP_DEBUG(CDialogManager, "showing message dialog (message: '", strMessage, "')");
    CMessageDlg(strMessage).DoModal(hWndParent);
}

void CDialogManager::ShowNotification(UINT stringID, int dialogID)
{
    ATL::CString strText;

    if(strText.LoadString(stringID))
        ShowNotification(strText, dialogID);
}

void CDialogManager::ShowNotification(ATL::CString const& strText, int dialogID)
{
    LOG_COMP_DEBUG(CDialogManager, "showing notification (text: '", strText, "'; ID: ", dialogID, ")");
    Show<CNotificationDlg>(false)->SetNotification(strText, dialogID);
}

void CDialogManager::RegisterDialog(int dialogID, create_function create)
{
    LOG_COMP_DEBUG(CDialogManager, "registering dialog (ID: ", dialogID, ")");

    if(boost::count(m_create_map | boost::adaptors::map_keys, dialogID))
        throw std::runtime_error("dialog already registered");

    m_create_map.emplace(dialogID, create);
}

CDialogManager::dialog_pointer CDialogManager::FindDialog(int dialogID) const
{
    dialog_map::const_iterator it = m_dialog_map.find(dialogID);
    return it != m_dialog_map.end() ? it->second : dialog_pointer();
}

dialog_pointer CDialogManager::CreateDialog(int dialogID)
{
    create_map::const_iterator it = m_create_map.find(dialogID);

    if(it == m_create_map.end())
        throw std::runtime_error("dialog not registered");

    dialog_pointer dialog = (*it)(GetDialog(IDD_MAIN));

    m_dialog_map.emplace(dialogID, dialog);

    return dialog;
}

void CDialogManager::DoDeferredDialogDestruction(int dialogID)
{
    dialog_map::iterator it = m_dialog_map.find(dialogID);

    if(it != m_dialog_map.end())
    {
        dialog_pointer dialog = it->second;

        if(!dialog || !dialog->IsWindow())
            m_dialog_map.erase(it);
    }
}

LRESULT CALLBACK CDialogManager::GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(HC_ACTION == nCode && lParam)
    {
        PMSG pMsg = reinterpret_cast<PMSG>(lParam);

        if(WM_DESTROY_DIALOG == pMsg->message && pMsg->lParam)
        {
            CDialogManager* dialogManager = reinterpret_cast<CDialogManager*>(pMsg->lParam);
            dialogManager->DoDeferredDialogDestruction(static_cast<int>(pMsg->wParam));
        }
    }

    return ::CallNextHookEx(nullptr, nCode, wParam, lParam);
}
