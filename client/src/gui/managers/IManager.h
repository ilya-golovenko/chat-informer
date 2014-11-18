//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once


class IManager
{
public:
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual ~IManager() = 0 {}
};
