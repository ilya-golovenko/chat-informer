//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerFactory.h>
#include <informer/managers/DialogManager.h>
#include <informer/common/Logging.h>

// STL headers
#include <exception>
#include <memory>


template
<
    typename Dialog,
    typename Function
>
class DialogHandlerInvoker
{
public:
    template <typename ...Args>
    static void Invoke(Function function, Args const& ...args)
    {
        if(std::shared_ptr<Dialog> dialog = FindDialog())
        {
            try
            {
                ((*dialog).*function)(args...);
            }
            catch(std::exception const& e)
            {
                LOG_COMP_ERROR(DialogHandlerInvoker, "caught exception: ", e);
            }
        }
    }

private:
    static std::shared_ptr<Dialog> FindDialog()
    {
        return CManagerFactory::Get<CDialogManager>().Find<Dialog>();
    }
};
