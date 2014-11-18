//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// ATL/WTL headers
#include <atlwin.h>

#ifndef __cplusplus
    #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLWIN_H__
    #error InformerProgressCtrl.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/controls/InformerControl.h>


template
<
    class T,
    class TBase = CStatic,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CInformerProgressCtrlImpl :
    public CInformerControlImpl<T, TBase, TWinTraits>
{
public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    void SetRange(int lower, int upper)
    {
        if(lower < upper)
        {
            m_lower = lower;
            m_upper = upper;

            if(m_pos < m_lower)
                m_pos = m_lower;

            if(m_pos > m_upper)
                m_pos = m_upper;

            Invalidate();
            UpdateWindow();
        }
    }

    void GetRange(int& lower, int& upper)
    {
        lower = m_lower;
        upper = m_upper;
    }

    void SetPos(int pos)
    {
        m_pos = pos;

        if(m_pos < m_lower)
            m_pos = m_lower;

        if(m_pos > m_upper)
            m_pos = m_upper;

        Invalidate();
        UpdateWindow();
    }

    int GetPos() const
    {
        return m_pos;
    }

    // Implementation

    CInformerProgressCtrlImpl() :
        m_lower(0),
        m_upper(100),
        m_pos(0)
    {
        EnableDoubleBuffering();
    }

    void DrawControl(CDCHandle dc, CRect& /*rc*/)
    {
        CRect rcClient;
        GetClientRect(rcClient);

        T* pT = static_cast<T*>(this);
        pT->DrawItem(dc, rcClient);
    }

    // Overrideables

    void DrawItem(CDCHandle dc, CRect& rcItem)
    {
        DrawRectangle(dc, rcItem, PEN_NORMAL, BRUSH_LIGHT);

        int pos = m_upper - m_pos;
        int range = m_upper - m_lower;

        rcItem.DeflateRect(2, 2);
        rcItem.right -= rcItem.Width() * pos / range;

        if(rcItem.Width() > 0)
            DrawRectangle(dc, rcItem, PEN_SELECTED, BRUSH_SELECTED);
    }

private:
    int m_lower;
    int m_upper;
    int m_pos;
};

class CInformerProgressCtrl :
    public CInformerProgressCtrlImpl<CInformerProgressCtrl>
{
public:
    DECLARE_WND_SUPERCLASS(L"InformerFrame", GetWndClassName())
};
