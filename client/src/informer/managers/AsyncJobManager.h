//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>
#include <informer/managers/AsyncJobBase.h>
#include <utility/concurrent_queue.hpp>

// STL headers
#include <thread>
#include <list>


class CAsyncJobManager : public CManagerBase<CAsyncJobManager>
{
friend class CAsyncJobThread;

public:
    CAsyncJobManager() = default;
    ~CAsyncJobManager() = default;

    virtual void Initialize();
    virtual void Finalize();

    void AddJob(CAsyncJobBase::Pointer asyncJob);

private:
    void ThreadExecute();
    void RunJob(CAsyncJobBase::Pointer asyncJob);

private:
    typedef chat::concurrent_queue<CAsyncJobBase::Pointer> async_job_queue;

private:
    std::list<std::thread> m_threads;
    async_job_queue m_asyncJobs;
};
