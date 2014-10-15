//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2012, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>

// STL headers
#include <functional>
#include <typeinfo>
#include <utility>
#include <memory>
#include <string>
#include <list>


class CManagerFactory
{
public:
    static void CreateManagers();
    static void InitializeManagers();
    static void FinalizeManagers();
    static void DestroyManagers();

public:
    CManagerFactory() = delete;
    ~CManagerFactory() = delete;

    template <typename T> static T& Get();
    template <typename T> static void Create();

private:
    typedef std::shared_ptr<IManager> manager_pointer;
    typedef std::function<manager_pointer()> create_function;
    typedef std::list<std::pair<std::string, manager_pointer>> manager_list;

private:
    static manager_pointer GetManager(std::type_info const& type_info);
    static void CreateManager(std::type_info const& type_info, create_function create);

private:
    static manager_list m_managers;
};

template <typename T> T& CManagerFactory::Get()
{
    return *std::static_pointer_cast<T>(GetManager(typeid(T)));
}

template <typename T> void CManagerFactory::Create()
{
    CreateManager(typeid(T), &T::Create);
}
