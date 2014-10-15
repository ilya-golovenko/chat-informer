//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/AsyncJobBase.h>

// BOOST headers
#include <boost/filesystem/path.hpp>

// STL headers
#include <string>


class CShellExecuteJob : public CAsyncJobBase
{
public:
    static Pointer Create(boost::filesystem::path const& filename,
                          boost::filesystem::path const& directory,
                          std::wstring const& parameters,
                          int showCommand);

public:
    CShellExecuteJob(boost::filesystem::path const& filename,
                     boost::filesystem::path const& directory,
                     std::wstring const& parameters,
                     int showCommand);

    virtual void Run();

private:
    boost::filesystem::path m_filename;
    boost::filesystem::path m_directory;
    std::wstring m_parameters;
    int m_showCommand;
};
