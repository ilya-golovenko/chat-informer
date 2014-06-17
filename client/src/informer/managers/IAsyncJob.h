//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once


class IAsyncJob
{
public:
    virtual void Run() = 0;
    virtual ~IAsyncJob() = 0 {}
};
