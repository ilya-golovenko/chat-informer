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
    #error InformerControl.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/utility/InformerHelper.h>
#include <informer/utility/MouseTrackHelper.h>


template
<
    class T,
    class TBase,
    class TWinTraits
>
class ATL_NO_VTABLE CInformerControlImpl :
    public CWindowImpl<T, TBase, TWinTraits>,
    public CMouseTrackHelper<T>,
    public CInformerHelper
{
public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    BOOL SubclassWindow(HWND hWnd)
    {
        ATLASSERT(m_hWnd == NULL);
        ATLASSERT(::IsWindow(hWnd));

        BOOL bRet = CWindowImpl<T, TBase, TWinTraits>::SubclassWindow(hWnd);

        if(bRet)
        {
            T* pT = static_cast<T*>(this);
            pT->Init();
        }

        return bRet;
    }

    void EnableDoubleBuffering()
    {
        m_doubleBuffered = true;
    }

    void DrawFillRect(CDCHandle dc, CRect const& rc, int brush)
    {
        dc.FillRect(rc, m_drawManager.Brush(brush));
    }

    void DrawRectangle(CDCHandle dc, CRect const& rc, int pen, int brush)
    {
        if(PEN_INVALID == pen)
        {
            DrawFillRect(dc, rc, brush);
        }
        else
        {
            SetupPenAndBrush(dc, pen, brush);
            dc.Rectangle(rc);
        }
    }

    void DrawRoundRect(CDCHandle dc, CRect const& rc, int pen, int brush)
    {
        SetupPenAndBrush(dc, pen, brush);
        dc.RoundRect(rc, CPoint(10, 10));
    }

    // Message map and handlers

    BEGIN_MSG_MAP(CInformerControlImpl)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_UPDATEUISTATE(OnUpdateUIState)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_PRINTCLIENT(OnPrintClient)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_ENABLE(OnEnable)
        MESSAGE_HANDLER_EX(WM_SETTEXT, OnSetText)
        CHAIN_MSG_MAP(CMouseTrackHelper<T>)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    int OnCreate(LPCREATESTRUCT /*lpCreateStruct*/)
    {
        T* pT = static_cast<T*>(this);
        pT->Init();
        return 0;
    }

    void OnUpdateUIState(WORD /*nAction*/, WORD /*nState*/)
    {
        // If the control is subclassed or superclassed, this message can cause
        // repainting without WM_PAINT. We don't use this state, so just do nothing.
    }

    BOOL OnEraseBkgnd(HDC /*hDC*/)
    {
        return TRUE;
    }

    void OnPrintClient(HDC hDC, UINT /*uFlags*/)
    {
        OnPaint(hDC);
    }

    void OnPaint(HDC hDC)
    {
        if(NULL != hDC)
        {
            CRect rcClient;
            GetClientRect(rcClient);
            DoDrawControl(hDC, rcClient);
        }
        else
        {
            CPaintDC dcPaint(m_hWnd);
            CRect rcPaint(dcPaint.m_ps.rcPaint);
            hDC = static_cast<HDC>(dcPaint);
            DoDrawControl(hDC, rcPaint);
        }
    }

    void OnEnable(BOOL /*bEnable*/)
    {
        Invalidate();
        UpdateWindow();
    }

    LRESULT OnSetText(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        SetRedraw(FALSE);

        LRESULT lRet = CWindowImpl<T, TBase, TWinTraits>::DefWindowProc(uMsg, wParam, lParam);

        SetRedraw(TRUE);

        Invalidate();
        UpdateWindow();

        return lRet;
    }

    // Implementation

    CInformerControlImpl() :
        m_doubleBuffered(false)
    {
    }

    void Init()
    {
        ATLASSERT(::IsWindow(m_hWnd));
    }

    void SetupPenAndBrush(CDCHandle dc, int pen, int brush)
    {
        if(PEN_INVALID == pen)
            dc.SelectStockPen(NULL_PEN);
        else
            dc.SelectPen(m_drawManager.Pen(pen));

        if(BRUSH_INVALID == brush)
            dc.SelectStockBrush(NULL_BRUSH);
        else
            dc.SelectBrush(m_drawManager.Brush(brush));
    }

    void DoDrawControl(CDCHandle dc, CRect& rc)
    {
        T* pT = static_cast<T*>(this);

        if(m_doubleBuffered)
        {
            CMemoryDC dcMemory(dc, rc);
            pT->DrawControl((HDC)dcMemory, rc);
        }
        else
        {
            pT->DrawControl(dc, rc);
        }
    }

    // Overrideables

    void DrawControl(CDCHandle /*dc*/, CRect& /*rc*/)
    {
        // must be implemented
        ATLASSERT(FALSE);
    }

protected:
    bool m_doubleBuffered;
};
