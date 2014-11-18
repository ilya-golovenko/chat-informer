//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Windows headers
#include <windows.h>


class CSaveDC
{
public:
    CSaveDC(HDC hDC) :
        m_hDC(hDC)
    {
        m_savedDC = ::SaveDC(m_hDC);
    }

    ~CSaveDC()
    {
        ::RestoreDC(m_hDC, m_savedDC);
    }

    CSaveDC(CSaveDC const&) = delete;
    CSaveDC& operator=(CSaveDC const&) = delete;

private:
    HDC m_hDC;
    int m_savedDC;
};
