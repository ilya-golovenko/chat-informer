//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Windows headers
#include <windows.h>

// BOOST headers
#include <boost/noncopyable.hpp>


class CAutoLocalFree : private boost::noncopyable
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

private:
    void* m_pointer;
};
