//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
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

    template <typename ...Args>
    void operator()(Args const& ...args)
    {
        DispatchHandler(DialogHandler<Dialog, Function, Invoker, Args...>(function_, args...));
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
