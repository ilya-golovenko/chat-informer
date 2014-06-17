//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
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


CEventManager::CEventManager()
{
}

void CEventManager::Initialize()
{
    LOG_INFO("initializing");
}

void CEventManager::Finalize()
{
    LOG_INFO("finalizing");
}

void CEventManager::AddEvent(missio::event::type event)
{
    LOG_DEBUG("adding event: ", event);

    if(missio::event::none != event)
    {
        CDialogManager& dialogManager = CManagerFactory::Get<CDialogManager>();

        if(!dialogManager.IsDialogVisible(GetEventDialogID(event)))
        {
            if(m_events.insert(event).second)
                m_event_added(event);
        }
    }
}

void CEventManager::RemoveEvent(missio::event::type event)
{
    LOG_DEBUG("removing event: ", event);

    if(missio::event::none != event)
    {
        if(m_events.erase(event) > 0)
            m_event_removed(event);
    }
}

void CEventManager::ClearEvents()
{
    LOG_DEBUG("clearing events");

    boost::for_each(m_events, boost::bind(boost::cref(m_event_removed), _1));

    m_events.clear();
}

bool CEventManager::ContainsEvents() const
{
    return !m_events.empty();
}

bool CEventManager::ContainsEvent(missio::event::type event) const
{
    return (m_events.find(event) != m_events.end());
}

missio::event::type CEventManager::GetEvent()
{
    if(!m_events.empty())
    {
        missio::event::type event = *m_events.begin();
        m_events.erase(m_events.begin());
        m_event_removed(event);
        return event;
    }

    return missio::event::none;
}

missio::event::type CEventManager::PeekEvent() const
{
    return (m_events.empty() ? missio::event::none : *m_events.begin());
}

void CEventManager::ShowEventNotification(missio::event::type event) const
{
    LOG_DEBUG("showing notification for event: ", event);

    if(missio::event::none != event)
    {
        CDialogManager& dialogManager = CManagerFactory::Get<CDialogManager>();

        int stringID = GetEventStringID(event);
        int dialogID = GetEventDialogID(event);

        if(!dialogManager.IsDialogVisible(dialogID))
            dialogManager.ShowNotification(stringID, dialogID);
    }
}

int CEventManager::GetEventStringID(missio::event::type event)
{
    switch(event)
    {
        case missio::event::version:    return IDS_EVENT_VERSION;
        case missio::event::forum:      return IDS_EVENT_FORUM;
        case missio::event::photoalbum: return IDS_EVENT_PHOTOALBUM;
        case missio::event::news:       return IDS_EVENT_NEWS;
    }

    throw std::runtime_error("unknown event type");
}

int CEventManager::GetEventDialogID(missio::event::type event)
{
    switch(event)
    {
        case missio::event::version:    return IDD_UPDATE;
        case missio::event::forum:      return IDD_FORUM;
        case missio::event::photoalbum: return IDD_PHOTOALBUM;
        case missio::event::news:       return IDD_NEWS;
    }

    return IDD_INVALID;
}
