//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
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
>
struct DialogBindTraits<void (Dialog::*)()>
{
    typedef void (Dialog::*FunctionType)();
    typedef DialogHandlerInvoker<Dialog, FunctionType> InvokerType;
    typedef DialogHandlerBinder<Dialog, FunctionType, InvokerType> BinderType;
};

template
<
    typename Dialog,
    typename A0
>
struct DialogBindTraits<void (Dialog::*)(A0)>
{
    typedef void (Dialog::*FunctionType)(A0);
    typedef DialogHandlerInvoker<Dialog, FunctionType> InvokerType;
    typedef DialogHandlerBinder<Dialog, FunctionType, InvokerType> BinderType;
};

template
<
    typename Dialog,
    typename A0,
    typename A1
>
struct DialogBindTraits<void (Dialog::*)(A0, A1)>
{
    typedef void (Dialog::*FunctionType)(A0, A1);
    typedef DialogHandlerInvoker<Dialog, FunctionType> InvokerType;
    typedef DialogHandlerBinder<Dialog, FunctionType, InvokerType> BinderType;
};
