//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/utility/DialogHandlerBinder.h>
#include <informer/utility/DialogHandlerInvoker.h>


template
<
    typename Function
>
struct DialogBindTraits;

template
<
    typename Dialog
    typename ...Args
>
struct DialogBindTraits<void (Dialog::*)(Args...)>
{
    typedef void (Dialog::*FunctionType)(Args...);
    typedef DialogHandlerInvoker<Dialog, FunctionType> InvokerType;
    typedef DialogHandlerBinder<Dialog, FunctionType, InvokerType> BinderType;
};

