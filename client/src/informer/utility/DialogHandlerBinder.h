//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/utility/DialogHandler.h>
#include <informer/managers/ManagerFactory.h>
#include <informer/managers/DispatchManager.h>

// STL headers
#include <utility>


template
<
    typename Dialog,
    typename Function,
    typename Invoker
>
class DialogHandlerBinder
{
public:
    DialogHandlerBinder(Function function) :
        function_(function)
    {
    }

    void operator()()
    {
        DispatchHandler(DialogHandler_0<Dialog, Function, Invoker>(function_));
    }

    template <typename Arg0>
    void operator()(Arg0 const& arg0)
    {
        DispatchHandler(DialogHandler_1<Dialog, Function, Invoker, Arg0>(function_, arg0));
    }

    template <typename Arg0, typename Arg1>
    void operator()(Arg0 const& arg0, Arg1 const& arg1)
    {
        DispatchHandler(DialogHandler_2<Dialog, Function, Invoker, Arg0, Arg1>(function_, arg0, arg1));
    }

private:
    template <typename Handler>
    static void DispatchHandler(Handler&& handler)
    {
        CManagerFactory::Get<CDispatchManager>().Dispatch(std::forward<Handler>(handler));
    }

private:
    Function function_;
};
