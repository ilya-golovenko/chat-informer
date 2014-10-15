//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/SkinManager.h>
#include <informer/common/Logging.h>


void CSkinManager::Initialize()
{
    LOG_COMP_INFO(CSkinManager, "initializing");

    // Load skin defined by configuration
}

void CSkinManager::Finalize()
{
    LOG_COMP_INFO(CSkinManager, "finalizing");

}

void CSkinManager::LoadSkin(std::string const& name)
{
}

std::vector<std::string> CSkinManager::GetSkinList() const
{
    std::vector<std::string> skins;
    boost::filesystem::path path("skins");

    if(boost::filesystem::exists(path))
    {
        boost::filesystem::directory_iterator first(path);
        boost::filesystem::directory_iterator last;

        for(; first != last; ++first)
        {
            if(boost::filesystem::is_directory(first->status()))
                skins.push_back(first->path().string());
        }
    }

    return skins;
}

std::string CSkinManager::GetImageFileName(int image) const
{
    return std::string();
}
