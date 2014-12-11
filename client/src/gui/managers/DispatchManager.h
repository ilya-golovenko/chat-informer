//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>

// BOOST headers
#include <boost/asio.hpp>


class CDispatchManager : public CManagerBase<CDispatchManager>
{
public:
    enum { WM_DISPATCH_HANDLER = WM_APP + 2003 };

public:
    CDispatchManager();
    ~CDispatchManager();

    void Initialize() override;
    void Finalize() override;

    template <typename Handler>
    void Dispatch(Handler const& handler);

private:
    void PostDispatchNotification();
    void ProcessDispatchedHandlers();

private:
    static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wparam, LPARAM lparam);

private:
    HHOOK m_messageHook;
    DWORD m_mainThreadID;

    boost::asio::io_service m_service;
};

template <typename Handler>
void CDispatchManager::Dispatch(Handler const& handler)
{
    m_service_.post(handler);
    PostDispatchNotification();
}
