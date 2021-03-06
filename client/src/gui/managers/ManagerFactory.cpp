//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2012, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/ManagerFactory.h>
#include <informer/managers/InformerManager.h>
#include <informer/managers/DialogManager.h>
#include <informer/managers/EventManager.h>
#include <informer/managers/DrawManager.h>
#include <informer/managers/IconManager.h>
#include <informer/managers/SoundManager.h>
#include <informer/managers/AsyncJobManager.h>
#include <informer/managers/DispatchManager.h>
#include <informer/common/Logging.h>


CManagerFactory::manager_map CManagerFactory::m_managers;

void CManagerFactory::CreateManagers()
{
    LOG_COMP_TRACE_FUNCTION(CManagerFactory);

    LOG_COMP_NOTICE(CManagerFactory, "creating managers");

    Create<CDrawManager>();
    Create<CIconManager>();
    Create<CSoundManager>();
    Create<CEventManager>();
    Create<CDialogManager>();
    Create<CDispatchManager>();
    Create<CAsyncJobManager>();
    Create<CInformerManager>();
}

void CManagerFactory::InitializeManagers()
{
    LOG_COMP_TRACE_FUNCTION(CManagerFactory);

    LOG_COMP_NOTICE(CManagerFactory, "initializing managers");

    boost::for_each(m_managers | boost::adaptors::map_values, boost::bind(&IManager::Initialize, _1));
}

void CManagerFactory::FinalizeManagers()
{
    LOG_COMP_TRACE_FUNCTION(CManagerFactory);

    LOG_COMP_NOTICE(CManagerFactory, "finalizing managers");

    for(auto const& manager : m_managers)
    {
        manager.second->Finalize();
    }

    boost::for_each(m_managers | boost::adaptors::map_values | boost::adaptors::reversed, boost::bind(&IManager::Finalize, _1));
}

void CManagerFactory::DestroyManagers()
{
    LOG_COMP_TRACE_FUNCTION(CManagerFactory);

    LOG_COMP_NOTICE(CManagerFactory, "destroying managers");

    m_managers.clear();
}

CManagerFactory::manager_pointer CManagerFactory::GetManager(std::type_info const& type_info)
{
    manager_list::const_iterator it = boost::find(m_managers | boost::adaptors::map_keys, type_info.name());

    if(it == m_managers.end())
        throw std::runtime_error("manager not found");

    return it->second;
}

void CManagerFactory::CreateManager(std::type_info const& type_info, create_function create)
{
    LOG_COMP_TRACE_FUNCTION(CManagerFactory);

    LOG_COMP_DEBUG(CManagerFactory, "creating manager: ", type_info.name());

    if(boost::count(m_managers | boost::adaptors::map_keys, type_info.name()))
        throw std::runtime_error("manager already created");

    m_managers.emplace_back(type_info.name(), create());
}
