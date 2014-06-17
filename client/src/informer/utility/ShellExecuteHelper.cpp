//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/utility/ShellExecuteHelper.h>
#include <informer/utility/ShellExecuteJob.h>
#include <informer/managers/ManagerFactory.h>
#include <informer/managers/AsyncJobManager.h>


CShellExecuteHelper::CShellExecuteHelper(boost::filesystem::path const& filename,
                                         boost::filesystem::path const& directory,
                                         std::wstring const& parameters,
                                         int showCommand) :
    m_filename(filename),
    m_directory(directory),
    m_parameters(parameters),
    m_showCommand(showCommand)
{
}

CShellExecuteHelper::CShellExecuteHelper(boost::filesystem::path const& filename,
                                         std::wstring const& parameters,
                                         int showCommand) :
    m_filename(filename),
    m_parameters(parameters),
    m_showCommand(showCommand)
{
}

CShellExecuteHelper::CShellExecuteHelper(boost::filesystem::path const& filename,
                                         int showCommand) :
    m_filename(filename),
    m_showCommand(showCommand)
{
}

void CShellExecuteHelper::Start()
{
    CAsyncJobManager& asyncJobManager = CManagerFactory::Get<CAsyncJobManager>();

    CShellExecuteJob::Pointer asyncJob = CShellExecuteJob::Create(
        m_filename, m_directory, m_parameters, m_showCommand);

    asyncJobManager.AddJob(asyncJob);
}
