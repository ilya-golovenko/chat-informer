//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>
#include <core/events.hpp>


class CEventManager :
    public CManagerBase<CEventManager>
{
public:
    CEventManager();

    virtual void Initialize();
    virtual void Finalize();

    void AddEvent(missio::event::type event);
    void RemoveEvent(missio::event::type event);
    void ClearEvents();

    bool ContainsEvents() const;
    bool ContainsEvent(missio::event::type event) const;

    missio::event::type GetEvent();
    missio::event::type PeekEvent() const;

    void ShowEventNotification(missio::event::type event) const;

    template <typename Handler>
    void OnEventAdded(Handler const& handler);

    template <typename Handler>
    void OnEventRemoved(Handler const& handler);

public:
    static int GetEventStringID(missio::event::type event);
    static int GetEventDialogID(missio::event::type event);

private:
    boost::signals2::signal<void (missio::event::type)> m_event_added;
    boost::signals2::signal<void (missio::event::type)> m_event_removed;

private:
    std::set<missio::event::type> m_events;
};

template <typename Handler>
void CEventManager::OnEventAdded(Handler const& handler)
{
    m_event_added.connect(handler);
}

template <typename Handler>
void CEventManager::OnEventRemoved(Handler const& handler)
{
    m_event_removed.connect(handler);
}
