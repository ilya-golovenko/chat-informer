//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Windows headers
#include <windows.h>


class CAutoLocalFree
{
public:
    explicit CAutoLocalFree(void* pointer) :
        m_pointer(pointer)
    {
    }

    ~CAutoLocalFree()
    {
        ::LocalFree(m_pointer);
    }

    CAutoLocalFree(CAutoLocalFree const&) = delete;
    CAutoLocalFree& operator=(CAutoLocalFree const&) = delete;

private:
    void* m_pointer;
};
