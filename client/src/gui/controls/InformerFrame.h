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
    #error InformerFrame.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/controls/InformerControl.h>
#include <informer/utility/SaveDC.h>


template
<
    class T,
    class TBase = CStatic,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CInformerFrameImpl :
    public CInformerControlImpl<T, TBase, TWinTraits>
{
public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Implementation

    void DrawControl(CDCHandle dc, CRect& /*rc*/)
    {
        CSaveDC saveDC(dc);

        CRect rcClient;
        GetClientRect(rcClient);

        T* pT = static_cast<T*>(this);
        pT->DrawItem(dc, rcClient);
    }

    // Overrideables

    void DrawItem(CDCHandle dc, CRect& rcItem)
    {
        DrawRoundRect(dc, rcItem, PEN_STATIC, BRUSH_INVALID);
    }
};

class CInformerFrame :
    public CInformerFrameImpl<CInformerFrame>
{
public:
    DECLARE_WND_SUPERCLASS(L"InformerFrame", GetWndClassName())
};
