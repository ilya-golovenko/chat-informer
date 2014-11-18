//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>


enum
{
    IMAGE_PHOTO_NONE,
    IMAGE_PHOTO_LOADING,

    IMAGE_COUNT
};

class CSkinManager : public CManagerBase<CSkinManager>
{
public:
    CSkinManager() = default;
    ~CSkinManager() = default;

    void Initialize() override;
    void Finalize() override;

    void LoadSkin(std::string const& name);

    std::vector<std::string> GetSkinList() const;
    std::string GetImageFileName(int image) const;
};
