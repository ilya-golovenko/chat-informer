//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerFactory.h>
#include <informer/managers/DrawManager.h>
#include <informer/managers/IconManager.h>
#include <informer/managers/SoundManager.h>
#include <informer/managers/EventManager.h>
#include <informer/managers/DialogManager.h>
#include <informer/managers/DispatchManager.h>
#include <informer/managers/AsyncJobManager.h>
#include <informer/managers/InformerManager.h>


//TODO: rename to CInformerContext
class CInformerHelper
{
public:
    CInformerHelper() :
        m_drawManager(CManagerFactory::Get<CDrawManager>()),
        m_iconManager(CManagerFactory::Get<CIconManager>()),
        m_soundManager(CManagerFactory::Get<CSoundManager>()),
        m_eventManager(CManagerFactory::Get<CEventManager>()),
        m_dialogManager(CManagerFactory::Get<CDialogManager>()),
        m_dispatchManager(CManagerFactory::Get<CDispatchManager>()),
        m_asyncJobManager(CManagerFactory::Get<CAsyncJobManager>()),
        m_informerManager(CManagerFactory::Get<CInformerManager>())
    {
    }

    CInformerHelper(CInformerHelper const&) = delete;
    CInformerHelper& operator=(CInformerHelper const&) = delete;

protected:
    CDrawManager& m_drawManager;
    CIconManager& m_iconManager;
    CSoundManager& m_soundManager;
    CEventManager& m_eventManager;
    CDialogManager& m_dialogManager;
    CDispatchManager& m_dispatchManager;
    CAsyncJobManager& m_asyncJobManager;
    CInformerManager& m_informerManager;
};
