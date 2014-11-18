//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/IManager.h>

// STL headers
#include <memory>


template <typename T>
class CManagerBase : public IManager
{
public:
    static std::shared_ptr<IManager> Create();
};

template <typename T>
std::shared_ptr<IManager> CManagerBase<T>::Create()
{
    return std::make_shared<T>();
}
