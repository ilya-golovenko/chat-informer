//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/utility/SingleInstance.h>


CSingleInstance::CSingleInstance() :
    m_hFileMapping(0)
{
    m_hFileMutex = ::CreateMutex(0, FALSE, GetMutexName());
    ATLVERIFY(0 != m_hFileMutex);
}

CSingleInstance::~CSingleInstance()
{
    if(0 != m_hFileMapping)
        ::CloseHandle(m_hFileMapping);

    ATLVERIFY(::CloseHandle(m_hFileMutex));
}

BOOL CSingleInstance::TrackFirstInstanceRunning(HWND hWndMain)
{
    if(!IsPreviousInstanceRunning())
    {
        m_hFileMapping = ::CreateFileMapping(INVALID_HANDLE_VALUE,
            0, PAGE_READWRITE, 0, sizeof(CWindowInstance), GetFileName());

        if(0 != m_hFileMapping)
        {
            CWindowInstance* lpWindowInstance = reinterpret_cast<CWindowInstance*>(
                ::MapViewOfFile(m_hFileMapping, FILE_MAP_WRITE, 0, 0, sizeof(CWindowInstance)));

            if(0 != lpWindowInstance)
            {
                DWORD dwWaitResult = ::WaitForSingleObject(m_hFileMutex, INFINITE);

                if(WAIT_OBJECT_0 == dwWaitResult)
                {
                    lpWindowInstance->hWndMain = hWndMain;

                    ATLVERIFY(::UnmapViewOfFile(lpWindowInstance));
                    ATLVERIFY(::ReleaseMutex(m_hFileMutex));

                    return TRUE;
                }

                ATLVERIFY(::UnmapViewOfFile(lpWindowInstance));
            }

            ::CloseHandle(m_hFileMapping);
            m_hFileMapping = 0;
        }
    }

    return FALSE;
}

BOOL CSingleInstance::IsPreviousInstanceRunning()
{
    HANDLE hFileMapping = ::OpenFileMapping(
        FILE_MAP_ALL_ACCESS, FALSE, GetFileName());

    BOOL bPreviousInstanceRunning = 0 != hFileMapping;

    if(0 != hFileMapping)
        ::CloseHandle(hFileMapping);

    return bPreviousInstanceRunning;
}

BOOL CSingleInstance::ActivatePreviousInstance()
{
    HANDLE hFileMapping = ::OpenFileMapping(
        FILE_MAP_ALL_ACCESS, FALSE, GetFileName());

    if(0 != hFileMapping)
    {
        CWindowInstance* lpWindowInstance = reinterpret_cast<CWindowInstance*>(
            ::MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, sizeof(CWindowInstance)));

        if(0 != lpWindowInstance)
        {
            DWORD dwWaitResult = ::WaitForSingleObject(m_hFileMutex, INFINITE);

            if(WAIT_OBJECT_0 == dwWaitResult)
            {
                CWindow wndMain(lpWindowInstance->hWndMain);

                if(wndMain.IsWindow())
                {
                    CWindow wndChild = wndMain.GetLastActivePopup();

                    if(!wndMain.IsWindowVisible())
                        wndMain.ShowWindow(SW_SHOW);

                    ::SetForegroundWindow(wndChild);

                    ATLVERIFY(::UnmapViewOfFile(lpWindowInstance));
                    ATLVERIFY(::ReleaseMutex(m_hFileMutex));
                    ATLVERIFY(::CloseHandle(hFileMapping));

                    return TRUE;
                }

                ATLVERIFY(::ReleaseMutex(m_hFileMutex));
            }

            ATLVERIFY(::UnmapViewOfFile(lpWindowInstance));
        }

        ATLVERIFY(::CloseHandle(hFileMapping));
    }

    return FALSE;
}

LPCWSTR CSingleInstance::GetMutexName()
{
    return L"Chat_Informer_{D5549B82-AD83-4fBC-82BE-8B19BF58835D}";
}

LPCWSTR CSingleInstance::GetFileName()
{
    return L"Chat_Informer_{F1D1F900-1A6A-46C5-B481-03BA364AD596}";
}
