//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>
#include <core/events.hpp>

// BOOST headers
#include <boost/signals2.hpp>

// STL headers
#include <set>


class CEventManager : public CManagerBase<CEventManager>
{
public:
    CEventManager() = default;
    ~CEventManager() = default;

    virtual void Initialize();
    virtual void Finalize();

    void AddEvent(chat::event::type event);
    void RemoveEvent(chat::event::type event);
    void ClearEvents();

    bool ContainsEvents() const;
    bool ContainsEvent(chat::event::type event) const;

    chat::event::type GetEvent();
    chat::event::type PeekEvent() const;

    void ShowEventNotification(chat::event::type event) const;

    template <typename Handler>
    void OnEventAdded(Handler const& handler);

    template <typename Handler>
    void OnEventRemoved(Handler const& handler);

public:
    static int GetEventStringID(chat::event::type event);
    static int GetEventDialogID(chat::event::type event);

private:
    boost::signals2::signal<void (chat::event::type)> m_event_added;
    boost::signals2::signal<void (chat::event::type)> m_event_removed;

private:
    std::set<chat::event::type> m_events;
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
