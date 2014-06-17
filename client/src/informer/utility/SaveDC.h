//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// BOOST headers
#include <boost/noncopyable.hpp>

// Windows headers
#include <windows.h>


class CSaveDC :
    private boost::noncopyable
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

private:
    HDC m_hDC;
    int m_savedDC;
};
