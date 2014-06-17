//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2012, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>


class CManagerFactory
{
public:
    static void CreateManagers();
    static void InitializeManagers();
    static void FinalizeManagers();
    static void DestroyManagers();

public:
    template <typename T>
    static inline T& Get() const;

    template <typename T>
    static inline void Create();

private:
    CManagerFactory();
    ~CManagerFactory();

private:
    typedef boost::shared_ptr<IManager> manager_pointer;
    typedef boost::function<manager_pointer()> create_function;
    typedef std::list<std::pair<std::string, manager_pointer> > manager_list;

private:
    static manager_pointer GetManager(std::type_info const& type_info) const;
    static void CreateManager(std::type_info const& type_info, create_function create);

private:
    static manager_list m_managers;
};

template <typename T>
inline T& CManagerFactory::Get() const
{
    return *boost::static_pointer_cast<T>(GetManager(typeid(T)));
}

template <typename T>
inline void CManagerFactory::Create()
{
    CreateManager(typeid(T), &T::Create);
}
