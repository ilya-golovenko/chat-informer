//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>
#include <informer/managers/AsyncJobBase.h>
#include <utility/concurrent_queue.hpp>

// BOOST headers
#include <boost/thread.hpp>


class CAsyncJobManager :
    public CManagerBase<CAsyncJobManager>
{
friend class CAsyncJobThread;

public:
    CAsyncJobManager();

    virtual void Initialize();
    virtual void Finalize();

    void AddJob(CAsyncJobBase::Pointer asyncJob);

private:
    void ThreadExecute();
    void RunJob(CAsyncJobBase::Pointer asyncJob);

private:
    typedef util::concurrent_queue<CAsyncJobBase::Pointer> async_job_queue;

private:
    async_job_queue m_asyncJobs;
    boost::thread_group m_threads;
};
