//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/resources/resource.h>
#include <informer/managers/ManagerBase.h>
#include <informer/common/Logging.h>

// BOOST headers
#include <boost/shared_ptr.hpp>


class CDialogManager :
    public CManagerBase<CDialogManager>
{
public:
    enum { WM_DESTROY_DIALOG = WM_APP + 2005 };

public:
    CDialogManager();
    virtual ~CDialogManager();

    virtual void Initialize();
    virtual void Finalize();

    ATL::CWindow CreateMainDialog();
    void DestroyMainDialog();

    ATL::CWindow GetMainDialog() const;

    void ShowDialog(int dialogID, bool bringToTop = true);
    void DestroyDialog(int dialogID);

    void SwitchDialogVisibility(int dialogID);
    bool IsDialogVisible(int dialogID) const;

    ATL::CWindow GetDialog(int dialogID) const;

    void ShowUserInfo(WTL::CString const& strNickName);
    void SendMessage(WTL::CString const& strNickName);

    void ShowMessage(UINT stringID, HWND hWndParent = NULL);
    void ShowMessage(WTL::CString const& strMessage, HWND hWndParent = NULL);

    void ShowNotification(UINT stringID, int dialogID);
    void ShowNotification(WTL::CString const& strText, int dialogID);

public:
    template <typename Dialog>
    void Register();

    template <typename Dialog>
    boost::shared_ptr<Dialog> Find() const;

    template <typename Dialog>
    boost::shared_ptr<Dialog> Get() const;

    template <typename Dialog>
    boost::shared_ptr<Dialog> Show(bool bringToTop = true);

private:
    typedef boost::shared_ptr<ATL::CWindow> dialog_pointer;
    typedef boost::function<dialog_pointer(HWND)> create_function;

private:
    typedef std::map<int, dialog_pointer> dialog_map;
    typedef std::map<int, create_function> create_map;

private:
    void RegisterDialog(int dialogID, create_function create);
    dialog_pointer FindDialog(int dialogID) const;
    dialog_pointer CreateDialog(int dialogID);

    void DoDeferredDialogDestruction(int dialogID);

private:
    static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    create_map m_create_map;
    dialog_map m_dialog_map;
    DWORD m_main_thread_id;
    HHOOK m_message_hook;
};

template <typename Dialog>
void CDialogManager::Register()
{
    RegisterDialog(Dialog::IDD, &Dialog::Create);
}

template <typename Dialog>
boost::shared_ptr<Dialog> CDialogManager::Find() const
{
    return boost::static_pointer_cast<Dialog>(FindDialog(Dialog::IDD));
}

template <typename Dialog>
boost::shared_ptr<Dialog> CDialogManager::Get() const
{
    boost::shared_ptr<Dialog> dialog = Find<Dialog>();

    if(!dialog)
        throw std::runtime_error("cannot find dialog");

    return dialog;
}

template <typename Dialog>
boost::shared_ptr<Dialog> CDialogManager::Show(bool bringToTop)
{
    ShowDialog(Dialog::IDD, bringToTop);
    return Get<Dialog>();
}
