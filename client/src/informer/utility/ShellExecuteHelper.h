//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// BOOST headers
#include <boost/filesystem/path.hpp>

// STL headers
#include <string>

// WIndows headers
#include <windows.h>


class CShellExecuteHelper
{
public:
    CShellExecuteHelper(boost::filesystem::path const& filename,
                        boost::filesystem::path const& directory,
                        std::wstring const& parameters,
                        int showCommand = SW_SHOW);

    CShellExecuteHelper(boost::filesystem::path const& filename,
                        std::wstring const& parameters,
                        int showCommand = SW_SHOW);

    explicit CShellExecuteHelper(boost::filesystem::path const& filename,
                                 int showCommand = SW_SHOW);

    void Start();

private:
    boost::filesystem::path m_filename;
    boost::filesystem::path m_directory;
    std::wstring m_parameters;
    int m_showCommand;
};
