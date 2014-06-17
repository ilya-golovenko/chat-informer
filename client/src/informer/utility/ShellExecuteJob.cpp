//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/utility/ShellExecuteJob.h>
#include <informer/utility/FormatErrorMessage.h>


CShellExecuteJob::Pointer CShellExecuteJob::Create(boost::filesystem::path const& filename,
                                                   boost::filesystem::path const& directory,
                                                   std::wstring const& parameters,
                                                   int m_showCommand)
{
    return boost::make_shared<CShellExecuteJob>(filename, directory, parameters, m_showCommand);
}

CShellExecuteJob::CShellExecuteJob(boost::filesystem::path const& filename,
                                   boost::filesystem::path const& directory,
                                   std::wstring const& parameters,
                                   int showCommand) :
    m_filename(filename),
    m_parameters(parameters),
    m_directory(directory),
    m_showCommand(showCommand)
{
}

void CShellExecuteJob::Run()
{
    SHELLEXECUTEINFO shellExecuteInfo = { 0 };

    shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shellExecuteInfo.fMask = SEE_MASK_FLAG_NO_UI;
    shellExecuteInfo.lpFile = m_filename.c_str();
    shellExecuteInfo.nShow = m_showCommand;
    shellExecuteInfo.lpVerb = L"open";

    if(!m_directory.empty())
        shellExecuteInfo.lpDirectory = m_directory.c_str();

    if(!m_parameters.empty())
        shellExecuteInfo.lpParameters = m_parameters.c_str();

    if(!::ShellExecuteEx(&shellExecuteInfo))
    {
        //std::wstring errorMessage = FormatErrorMessage(::GetLastError());
        //TODO: throw std::runtime_error("ShellExecuteEx failed: " + errorMessage);
    }
}
