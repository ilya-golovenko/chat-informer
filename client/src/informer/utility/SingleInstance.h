//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Windows headers
#include <windows.h>


class CSingleInstance
{
public:
    struct CWindowInstance
    {
        HWND hWndMain;
    };

public:
    CSingleInstance();
    ~CSingleInstance();

    BOOL TrackFirstInstanceRunning(HWND hWndMain);
    BOOL IsPreviousInstanceRunning();
    BOOL ActivatePreviousInstance();

public:
    static LPCWSTR GetMutexName();
    static LPCWSTR GetFileName();

private:
    HANDLE m_hFileMutex;
    HANDLE m_hFileMapping;
};
