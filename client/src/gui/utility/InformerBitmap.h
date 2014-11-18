//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// ATL/WTL headers
#include <atlbase.h>
#include <atlwin.h>


#ifndef __cplusplus
    #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLWIN_H__
    #error InformerBitmap.h requires atlwin.h to be included first
#endif


template <bool t_bManaged>
class CInformerBitmapT :
    public CBitmapT<t_bManaged>
{
public:
    CInformerBitmapT(HBITMAP hBitmap = NULL) :
        CBitmapT(hBitmap)
    {
    }

    ~CInformerBitmapT()
    {
        if(t_bManaged && m_hBitmap != NULL)
            DeleteObject();
    }

    CInformerBitmapT<t_bManaged>& operator=(HBITMAP hBitmap)
    {
        Attach(hBitmap);
        return *this;
    }

    HBITMAP LoadFromFile(LPCWSTR lpszFileName)
    {
        ATLASSERT(m_hBitmap == NULL);
        ATLASSERT(lpszFileName != NULL);

        CComPtr<IPicture> spPicture;
        WCHAR szFileName[MAX_PATH] = { 0 };

        if(::PathIsRelative(lpszFileName))
            ::GetCurrentDirectory(MAX_PATH, szFileName);

        ::PathAppend(szFileName, lpszFileName);
        CT2OLEEX<MAX_PATH> fileName(szFileName);

        HRESULT hRes = ::OleLoadPicturePath(fileName,
            0, 0, 0, IID_IPicture, (LPVOID*)&spPicture);

        if(SUCCEEDED(hRes))
            CreateFromPicture(spPicture);

        return m_hBitmap;
    }

    HBITMAP LoadFromResource(ATL::_U_STRINGorID ID)
    {
        ATLASSERT(m_hBitmap == NULL);

        CResource resPicture;

        if(resPicture.Load(RT_RCDATA, ID))
        {
            DWORD dwSize = resPicture.GetSize();

            if(dwSize > 0)
            {
                LPVOID lpData = resPicture.Lock();

                if(lpData)
                    LoadFromMemory(lpData, dwSize);
            }
        }

        return m_hBitmap;
    }

    HBITMAP LoadFromMemory(LPCVOID lpData, DWORD dwSize)
    {
        ATLASSERT(m_hBitmap == NULL);
        ATLASSERT(lpData != NULL && dwSize > 0);

        HGLOBAL hPicture = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);

        if(!hPicture)
            return NULL;

        LPVOID lpBuffer = ::GlobalLock(hPicture);

        if(!lpBuffer)
        {
            ::GlobalFree(hPicture);
            return NULL;
        }

        ::MoveMemory(lpBuffer, lpData, dwSize);
        ::GlobalUnlock(hPicture);

        CComPtr<IStream> spStream;
        CComPtr<IPicture> spPicture;

        HRESULT hRes = ::CreateStreamOnHGlobal(hPicture, TRUE, &spStream);

        if(FAILED(hRes))
        {
            ::GlobalFree(hPicture);
            return NULL;
        }

        hRes = ::OleLoadPicture(spStream, dwSize,
            FALSE, IID_IPicture, (LPVOID*)&spPicture);

        if(SUCCEEDED(hRes))
            CreateFromPicture(spPicture);

        return m_hBitmap;
    }

    HBITMAP CreateFromPicture(LPPICTURE lpPicture)
    {
        ATLASSERT(m_hBitmap == NULL);
        ATLASSERT(lpPicture != NULL);

        CDC dcBitmap;

        if(dcBitmap.CreateCompatibleDC())
        {
            OLE_XSIZE_HIMETRIC hmWidth;
            OLE_YSIZE_HIMETRIC hmHeight;

            lpPicture->get_Width(&hmWidth);
            lpPicture->get_Height(&hmHeight);

            CSize size(hmWidth, hmHeight);
            dcBitmap.HIMETRICtoDP(&size);

            if(CreateBitmap(size.cx, size.cy, 1, 32, NULL))
            {
                CBitmapHandle oldBitmap = dcBitmap.SelectBitmap(m_hBitmap);

                lpPicture->Render(dcBitmap, 0, 0, size.cx, size.cy,
                    0, hmHeight, hmWidth, -hmHeight, NULL);

                dcBitmap.SelectBitmap(oldBitmap);
            }
        }

        return m_hBitmap;
    }
};

typedef CInformerBitmapT<false>         CInformerBitmapHandle;
typedef CInformerBitmapT<true>          CInformerBitmap;
