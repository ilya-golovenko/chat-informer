//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

#include <informer/managers/AsyncJobManager.h>
#include <informer/common/Logging.h>


void CAsyncJobManager::Initialize()
{
    LOG_COMP_TRACE_FUNCTION(CDialogManager);

    LOG_COMP_INFO(CAsyncJobManager, "initializing");

    m_threads.emplace_back(std::thread(&CAsyncJobManager::ThreadExecute, this));
    m_threads.emplace_back(std::thread(&CAsyncJobManager::ThreadExecute, this));
}

void CAsyncJobManager::Finalize()
{
    LOG_COMP_TRACE_FUNCTION(CDialogManager);

    LOG_COMP_INFO(CAsyncJobManager, "finalizing");

    m_asyncJobs.disable(true);

    for(std::thread& thread : m_threads)
    {
        if(thread.joinable())
        {
            thread.join();
        }
    }
}

void CAsyncJobManager::AddJob(CAsyncJobBase::Pointer asyncJob)
{
    LOG_COMP_DEBUG(CAsyncJobManager, "adding new async job");

    m_asyncJobs.push(asyncJob);
}

void CAsyncJobManager::ThreadExecute()
{
    LOG_COMP_TRACE_FUNCTION(CDialogManager);

    LOG_COMP_DEBUG(CAsyncJobManager, "async job thread started");

    for(CAsyncJobBase::Pointer asyncJob; m_asyncJobs.pop(asyncJob); )
    {
        RunJob(asyncJob);
    }

    LOG_COMP_DEBUG(CAsyncJobManager, "async job thread terminated");
}

void CAsyncJobManager::RunJob(CAsyncJobBase::Pointer asyncJob)
{
    LOG_COMP_TRACE_FUNCTION(CDialogManager);

    LOG_COMP_DEBUG(CAsyncJobManager, "starting async job");

    try
    {
        asyncJob->Run();
    }
    catch(std::exception const& e)
    {
        LOG_COMP_ERROR(CAsyncJobManager, "caught exception: ", e);
    }
}
