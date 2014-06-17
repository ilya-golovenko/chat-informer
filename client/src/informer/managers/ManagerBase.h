//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/IManager.h>

// BOOST headers
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>


template <typename T>
class CManagerBase :
    public IManager,
    private boost::noncopyable,
    public boost::enable_shared_from_this<T>
{
public:
    static boost::shared_ptr<IManager> Create();
};

template <typename T>
boost::shared_ptr<IManager> CManagerBase<T>::Create()
{
    return boost::make_shared<T>();
}
