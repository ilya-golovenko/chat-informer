//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/IAsyncJob.h>

// STL headers
#include <memory>


class CAsyncJobBase : public IAsyncJob
{
public:
    typedef std::shared_ptr<IAsyncJob> Pointer;

public:
    CAsyncJobBase(CAsyncJobBase const&) = delete;
    CAsyncJobBase& operator=(CAsyncJobBase const&) = delete;
};
