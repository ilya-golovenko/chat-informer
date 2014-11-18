//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Windows headers
#include <windows.h>


struct CScrollInfo : SCROLLINFO
{
    CScrollInfo(UINT _fMask, int _nMin = 0, int _nMax = 0, UINT _nPage = 0, int _nPos = 0)
    {
        ::ZeroMemory(this, sizeof(SCROLLINFO));
        cbSize = sizeof(SCROLLINFO);

        fMask = _fMask;
        nMin = _nMin;
        nMax = _nMax;
        nPage = _nPage;
        nPos = _nPos;
    }

    operator LPSCROLLINFO()
    {
        return this;
    }
};
