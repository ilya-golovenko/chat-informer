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
    #error InformerThumbnail.h requires atlwin.h to be included first
#endif

// Application headers
#include <informer/controls/InformerControl.h>
#include <informer/utility/InformerBitmap.h>


template
<
    class T,
    class TBase = CStatic,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CInformerThumbnailImpl :
    public CInformerControlImpl<T, TBase, TWinTraits>
{
public:
    static UINT const drawTextFormat = DT_NOCLIP
        | DT_CENTER | DT_EXPANDTABS | DT_WORDBREAK;

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    BOOL LoadImageFromFile(LPCWSTR lpszFileName)
    {
        if(!m_bitmap.IsNull())
            m_bitmap.DeleteObject();

        if(!m_bitmap.LoadFromFile(lpszFileName))
            return FALSE;

        if(!m_bitmap.GetSize(m_sizeBitmap))
            return FALSE;

        return RefreshWindow();
    }

    BOOL LoadImageFromResource(ATL::_U_STRINGorID resource)
    {
        if(!m_bitmap.IsNull())
            m_bitmap.DeleteObject();

        if(!m_bitmap.LoadFromResource(resource))
        {
            if(!m_bitmap.LoadBitmap(resource))
                return FALSE;
        }

        if(!m_bitmap.GetSize(m_sizeBitmap))
            return FALSE;

        return RefreshWindow();
    }

    BOOL RefreshWindow()
    {
        return RedrawWindow(NULL, NULL,
            RDW_INVALIDATE | RDW_UPDATENOW);
    }

    // CInformerControl

    void DrawControl(CDCHandle dc, CRect& /*rc*/)
    {
        CRect rcClient;
        GetClientRect(rcClient);

        T* pT = static_cast<T*>(this);
        pT->DrawItem(dc, rcClient);
    }

    void DrawItem(CDCHandle dc, CRect& rcItem)
    {
        DrawFillRect(dc, rcItem, BRUSH_LIGHT);

        if(!m_bitmap.IsNull())
        {
            CDC dcBitmap;

            if(dcBitmap.CreateCompatibleDC(dc))
            {
                dcBitmap.SelectBitmap(m_bitmap);

                int cx = ::MulDiv(m_sizeBitmap.cx,
                    rcItem.Height(), m_sizeBitmap.cy);

                int cy = ::MulDiv(m_sizeBitmap.cy,
                    rcItem.Width(), m_sizeBitmap.cx);

                CSize newBitmapSize(
                    min(cx, rcItem.Width()),
                    min(cy, rcItem.Height()));

                CPoint pt = rcItem.TopLeft();
                pt += (rcItem.Size() - newBitmapSize) / 2;

                if(newBitmapSize == m_sizeBitmap)
                {
                    dc.BitBlt(pt.x, pt.y, m_sizeBitmap.cx,
                        m_sizeBitmap.cy, dcBitmap, 0, 0, SRCCOPY);
                }
                else
                {
                    dc.SetStretchBltMode(STRETCH_HALFTONE);
                    dc.StretchBlt(pt.x, pt.y, newBitmapSize.cx, newBitmapSize.cy,
                        dcBitmap, 0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy, SRCCOPY);
                }
            }
        }

        DrawRectangle(dc, rcItem, PEN_STATIC, BRUSH_INVALID);
    }

    // Implementation

    CInformerThumbnailImpl()
    {
        EnableDoubleBuffering();
    }

private:
    CSize m_sizeBitmap;
    CInformerBitmap m_bitmap;
};

class CInformerThumbnail :
    public CInformerThumbnailImpl<CInformerThumbnail>
{
public:
    DECLARE_WND_SUPERCLASS(L"InformerThumbnail", GetWndClassName())
};
