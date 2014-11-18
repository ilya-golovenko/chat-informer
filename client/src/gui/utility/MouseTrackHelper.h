//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// ATL/WTL headers
#include <atlwin.h>

// Windows headers
#include <windows.h>

#ifndef __cplusplus
    #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLWIN_H__
    #error MouseTrackHelper.h requires atlwin.h to be included first
#endif


template <class T>
class CMouseTrackHelper
{
public:
    struct CTrackMouseEvent : TRACKMOUSEEVENT
    {
        CTrackMouseEvent()
        {
            ::ZeroMemory(this, sizeof(TRACKMOUSEEVENT));
            cbSize = sizeof(TRACKMOUSEEVENT);
        }
    };

public:
    // Operations

    bool IsMouseOver() const
    {
        return m_tracking;
    }

    // Message map and handlers

    BEGIN_MSG_MAP(CMouseTrackHelper<T>)
        MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
        MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
    END_MSG_MAP()

    LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
    {
        T* pT = static_cast<T*>(this);
        pT->SetMsgHandled(FALSE);

        if(!m_tracking)
        {
            CTrackMouseEvent trackMouseEvent;

            trackMouseEvent.dwFlags = TME_LEAVE;
            trackMouseEvent.hwndTrack = pT->m_hWnd;
            trackMouseEvent.dwHoverTime = HOVER_DEFAULT;

            if(::_TrackMouseEvent(&trackMouseEvent))
            {
                m_tracking = true;
                pT->HandleMouseEnter(CPoint(lParam));
            }
        }

        bHandled = pT->IsMsgHandled();
        return static_cast<LRESULT>(FALSE);
    }

    LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        T* pT = static_cast<T*>(this);
        pT->SetMsgHandled(FALSE);

        if(m_tracking)
        {
            m_tracking = false;
            pT->HandleMouseLeave();
        }

        bHandled = pT->IsMsgHandled();
        return static_cast<LRESULT>(FALSE);
    }

    // Implementation

    CMouseTrackHelper() :
        m_tracking(false)
    {
    }

    // Overrideables

    void HandleMouseEnter(CPoint /*pt*/)
    {
        // can be implemented
    }

    void HandleMouseLeave()
    {
        // can be implemented
    }

private:
    bool m_tracking;
};
