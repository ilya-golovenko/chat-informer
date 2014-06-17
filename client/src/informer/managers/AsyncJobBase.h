//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/IAsyncJob.h>

// BOOST headers
#include <boost/noncopyable.h>
#include <boost/shared_ptr.h>


class CAsyncJobBase :
    private boost::noncopyable,
    public IAsyncJob
{
public:
    typedef boost::shared_ptr<IAsyncJob> Pointer;
};
