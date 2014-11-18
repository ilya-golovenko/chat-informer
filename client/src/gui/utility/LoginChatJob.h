//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/AsyncJobBase.h>
#include <informer/utility/InformerHelper.h>


class CLoginChatJob :
    public CAsyncJobBase,
    public CInformerHelper
{
public:
    static Pointer Create();

public:
    virtual void Run();
};
