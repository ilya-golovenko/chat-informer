//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/EventManager.h>
#include <informer/managers/ManagerFactory.h>
#include <informer/managers/DialogManager.h>
#include <informer/controls/InformerDialog.h>
#include <informer/common/Logging.h>

// BOOST headers
#include <boost/range.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

// STL headers
#include <stdexcept>


void CEventManager::Initialize()
{
    LOG_COMP_TRACE_FUNCTION(CEventManager);

    LOG_COMP_INFO(CEventManager, "initializing");
}

void CEventManager::Finalize()
{
    LOG_COMP_TRACE_FUNCTION(CEventManager);

    LOG_COMP_INFO(CEventManager, "finalizing");
}

void CEventManager::AddEvent(chat::event::type event)
{
    LOG_COMP_DEBUG(CEventManager, "adding event: ", event);

    if(chat::event::none != event)
    {
        CDialogManager& dialogManager = CManagerFactory::Get<CDialogManager>();

        if(!dialogManager.IsDialogVisible(GetEventDialogID(event)))
        {
            if(m_events.insert(event).second)
                m_event_added(event);
        }
    }
}

void CEventManager::RemoveEvent(chat::event::type event)
{
    LOG_COMP_DEBUG(CEventManager, "removing event: ", event);

    if(chat::event::none != event)
    {
        if(m_events.erase(event) > 0)
            m_event_removed(event);
    }
}

void CEventManager::ClearEvents()
{
    LOG_COMP_DEBUG(CEventManager, "clearing events");

    boost::for_each(m_events, boost::bind(boost::cref(m_event_removed), _1));

    m_events.clear();
}

bool CEventManager::ContainsEvents() const
{
    return !m_events.empty();
}

bool CEventManager::ContainsEvent(chat::event::type event) const
{
    return m_events.find(event) != m_events.end();
}

chat::event::type CEventManager::GetEvent()
{
    if(!m_events.empty())
    {
        chat::event::type event = *m_events.begin();
        m_events.erase(m_events.begin());
        m_event_removed(event);
        return event;
    }

    return chat::event::none;
}

chat::event::type CEventManager::PeekEvent() const
{
    return m_events.empty() ? chat::event::none : *m_events.begin();
}

void CEventManager::ShowEventNotification(chat::event::type event) const
{
    LOG_COMP_DEBUG(CEventManager, "showing notification for event: ", event);

    if(chat::event::none != event)
    {
        CDialogManager& dialogManager = CManagerFactory::Get<CDialogManager>();

        int stringID = GetEventStringID(event);
        int dialogID = GetEventDialogID(event);

        if(!dialogManager.IsDialogVisible(dialogID))
            dialogManager.ShowNotification(stringID, dialogID);
    }
}

int CEventManager::GetEventStringID(chat::event::type event)
{
    switch(event)
    {
        case chat::event::version:    return IDS_EVENT_VERSION;
        case chat::event::forum:      return IDS_EVENT_FORUM;
        case chat::event::photoalbum: return IDS_EVENT_PHOTOALBUM;
        case chat::event::news:       return IDS_EVENT_NEWS;
    }

    throw std::runtime_error("unknown event type");
}

int CEventManager::GetEventDialogID(chat::event::type event)
{
    switch(event)
    {
        case chat::event::version:    return IDD_UPDATE;
        case chat::event::forum:      return IDD_FORUM;
        case chat::event::photoalbum: return IDD_PHOTOALBUM;
        case chat::event::news:       return IDD_NEWS;
    }

    return IDD_INVALID;
}
