//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

#include <informer/managers/AsyncJobManager.h>
#include <informer/common/Logging.h>


CAsyncJobManager::CAsyncJobManager()
{
}

void CAsyncJobManager::Initialize()
{
    LOG_INFO("initializing");

    m_threads.create_thread(boost::bind(&CAsyncJobManager::ThreadExecute, this));
    m_threads.create_thread(boost::bind(&CAsyncJobManager::ThreadExecute, this));
}

void CAsyncJobManager::Finalize()
{
    LOG_INFO("finalizing");

    m_asyncJobs.disable(true);
    m_threads.join_all();
}

void CAsyncJobManager::AddJob(CAsyncJobBase::Pointer asyncJob)
{
    LOG_DEBUG("adding new async job");

    m_asyncJobs.push(asyncJob);
}

void CAsyncJobManager::ThreadExecute()
{
    LOG_DEBUG("async job thread started");

    for(CAsyncJobBase::Pointer asyncJob;
        m_asyncJobs.pop(asyncJob); )
    {
        RunJob(asyncJob);
    }

    LOG_DEBUG("async job thread terminated");
}

void CAsyncJobManager::RunJob(CAsyncJobBase::Pointer asyncJob)
{
    LOG_TRACE_SCOPE();
    LOG_DEBUG("starting async job");

    try
    {
        asyncJob->Run();
    }
    catch(std::exception const& e)
    {
        LOG_ERROR("caught exception: ", e);
    }
}
